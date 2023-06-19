#include "mFileSystem.h"

//  xdrv_22_sonoff_ifan.ino - sonoff iFan02 and iFan03 support for Tasmota

#ifdef USE_MODULE_DRIVERS_FILESYSTEM


const char* mFileSystem::PM_MODULE_DRIVERS_FILESYSTEM_CTR = D_MODULE_DRIVERS_FILESYSTEM_CTR;
const char* mFileSystem::PM_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR = D_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR;

/*********************************************************************************************\
This driver adds universal file system support for
- ESP8266 (sd card or littlefs on  > 1 M devices with special linker file e.g. eagle.flash.4m2m.ld)
  (makes no sense on 1M devices without sd card)
- ESP32 (sd card or littlefs or sfatfile system).

The sd card chip select is the standard SDCARD_CS or when not found SDCARD_CS_PIN and initializes
the FS System Pointer ufsp which can be used by all standard file system calls.

The only specific call is UfsInfo() which gets the total size (0) and free size (1).

A button is created in the setup section to show up the file directory to download and upload files
subdirectories are supported.

Supported commands:
ufs       fs info
ufstype   get filesytem type 0=none 1=SD  2=Flashfile
ufssize   total size in kB
ufsfree   free size in kB
\*********************************************************************************************/

#define XDRV_50           50

#ifndef SDCARD_CS_PIN
#define SDCARD_CS_PIN     4
#endif

#define UFS_TNONE         0
#define UFS_TSDC          1
#define UFS_TFAT          2
#define UFS_TLFS          3

/*
// In tasmota.ino
#ifdef ESP8266
#include <LittleFS.h>
#include <SPI.h>
#ifdef USE_SDCARD
#include <SD.h>
#include <SDFAT.h>
#endif  // USE_SDCARD
#endif  // ESP8266
#ifdef ESP32
#include <LITTLEFS.h>
#ifdef USE_SDCARD
#include <SD.h>
#endif  // USE_SDCARD
#include "FFat.h"
#include "FS.h"
#endif  // ESP32
*/

// Global file system pointer
FS *ufsp;
// Flash file system pointer
FS *ffsp;
// Local pointer for file managment
FS *dfsp;

char ufs_path[48];
File ufs_upload_file;
uint8_t ufs_dir;
// 0 = None, 1 = SD, 2 = ffat, 3 = littlefs
uint8_t ufs_type;
uint8_t ffs_type;

struct {
  char run_file[48];
  int run_file_pos = -1;
  bool run_file_mutex = 0;
  bool download_busy;
} UfsData;



  // /**
  //  * @brief Move into SDCard code later
  //  * 
  //  */
  // SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  // if (!SD.begin(SD_CS)) {
  //   ALOG_INF(PSTR("SDCard MOUNT FAIL"));
  // } else {
  //   uint32_t cardSize = SD.cardSize() / (1024 * 1024);
  //   String str = "SDCard Size: " + String(cardSize) + "MB";
  //   ALOG_INF(PSTR("%s"),str.c_str());
  // }



/*********************************************************************************************/

// // Init flash file system
void mFileSystem::UfsInitOnce(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("UFS: mFileSystem::UfsInitOnce"));
  ufs_type = 0;
  ffsp = 0;
  ufs_dir = 0;

#ifdef ESP8266
  ffsp = &LittleFS;
  if (!LittleFS.begin()) {
  	AddLog(LOG_LEVEL_INFO, PSTR("UFS: !LittleFS.begin()"));
    ffsp = 0;
    return;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("UFS: LittleFS.begin()"));
#endif  // ESP8266

// #ifdef ESP32
//   // try lfs first
//   ffsp = &LITTLEFS;
//   if (!LITTLEFS.begin(true)) {
//     // ffat is second
//     ffsp = &FFat;
//     if (!FFat.begin(true)) {
//       return;
//     }
//     ffs_type = UFS_TFAT;
//     ufs_type = ffs_type;
//     ufsp = ffsp;
//     dfsp = ffsp;
//     return;
//   }
// #endif // ESP32
  ffs_type = UFS_TLFS;
  ufs_type = ffs_type;
  ufsp = ffsp;
  dfsp = ffsp;
}

// Called from tasmota.ino at restart. This inits flash file only
void mFileSystem::UfsInit(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("UFS: mFileSystem::UfsInit"));
  UfsData.run_file_pos = -1;
  UfsInitOnce();
  if (ufs_type) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: FlashFS mounted with %d kB free"), UfsInfo(1, 0));
  }
}

// #ifdef USE_SDCARD
// void mFileSystem::UfsCheckSDCardInit(void) {
//   if (TasmotaGlobal.spi_enabled) {
//     int8_t cs = SDCARD_CS_PIN;
//     if (PinUsed(GPIO_SDCARD_CS)) {
//       cs = Pin(GPIO_SDCARD_CS);
//     }

// #ifdef EPS8266
//     SPI.begin();
// #endif // EPS8266

// #ifdef ESP32
//     SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
// #endif // ESP32

//     if (SD.begin(cs)) {
// #ifdef ESP8266
//       ufsp = &SDFS;
// #endif  // ESP8266

// #ifdef ESP32
//       ufsp = &SD;
// #endif  // ESP32
//       ufs_type = UFS_TSDC;
//       dfsp = ufsp;
//       if (ffsp) {ufs_dir = 1;}
//       // make sd card the global filesystem
// #ifdef ESP8266
//       // on esp8266 sdcard info takes several seconds !!!, so we ommit it here
//       AddLog(LOG_LEVEL_INFO, PSTR("UFS: SDCard mounted"));
// #endif // ESP8266
// #ifdef ESP32
//       AddLog(LOG_LEVEL_INFO, PSTR("UFS: SDCard mounted with %d kB free"), UfsInfo(1, 0));
// #endif // ESP32
//     }
//   }
// }
// #endif // USE_SDCARD

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

// #if USE_LONG_FILE_NAMES>0
// #undef REJCMPL
// #define REJCMPL 6
// #else
// #undef REJCMPL
// #define REJCMPL 8
// #endif

// uint8_t mFileSystem::UfsReject(char *name) {
//   char *lcp = strrchr(name,'/');
//   if (lcp) {
//     name = lcp + 1;
//   }

//   while (*name=='/') { name++; }
//   if (*name=='_') { return 1; }
//   if (*name=='.') { return 1; }

//   if (!strncasecmp(name, "SPOTLI~1", REJCMPL)) { return 1; }
//   if (!strncasecmp(name, "TRASHE~1", REJCMPL)) { return 1; }
//   if (!strncasecmp(name, "FSEVEN~1", REJCMPL)) { return 1; }
//   if (!strncasecmp(name, "SYSTEM~1", REJCMPL)) { return 1; }
//   if (!strncasecmp(name, "System Volume", 13)) { return 1; }
//   return 0;
// }

// /*********************************************************************************************\
//  * Tfs low level functions
// \*********************************************************************************************/

// bool mFileSystem::TfsFileExists(const char *fname){
//   if (!ffs_type) { return false; }

//   bool yes = ffsp->exists(fname);
//   if (!yes) {
//     AddLog(LOG_LEVEL_DEBUG, PSTR("TFS: File not found"));
//   }
//   return yes;
// }

// bool mFileSystem::TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len) {
//   if (!ffs_type) { return false; }

//   File file = ffsp->open(fname, "w");
//   if (!file) {
//     AddLog(LOG_LEVEL_INFO, PSTR("TFS: Save failed"));
//     return false;
//   }

//   file.write(buf, len);
//   file.close();
//   return true;
// }

// bool mFileSystem::TfsInitFile(const char *fname, uint32_t len, uint8_t init_value) {
//   if (!ffs_type) { return false; }

//   File file = ffsp->open(fname, "w");
//   if (!file) {
//     AddLog(LOG_LEVEL_INFO, PSTR("TFS: Erase failed"));
//     return false;
//   }

//   for (uint32_t i = 0; i < len; i++) {
//     file.write(&init_value, 1);
//   }
//   file.close();
//   return true;
// }

// bool mFileSystem::TfsLoadFile(const char *fname, uint8_t *buf, uint32_t len) {
//   if (!ffs_type) { return false; }
//   if (!TfsFileExists(fname)) { return false; }

//   File file = ffsp->open(fname, "r");
//   if (!file) {
//     AddLog(LOG_LEVEL_INFO, PSTR("TFS: File not found"));
//     return false;
//   }

//   file.read(buf, len);
//   file.close();
//   return true;
// }

// bool mFileSystem::TfsDeleteFile(const char *fname) {
//   if (!ffs_type) { return false; }

//   if (!ffsp->remove(fname)) {
//     AddLog(LOG_LEVEL_INFO, PSTR("TFS: Delete failed"));
//     return false;
//   }
//   return true;
// }

// bool mFileSystem::TfsRenameFile(const char *fname1, const char *fname2) {
//   if (!ffs_type) { return false; }

//   if (!ffsp->rename(fname1, fname2)) {
//     AddLog(LOG_LEVEL_INFO, PSTR("TFS: Rename failed"));
//     return false;
//   }
//   return true;
// }

// /*********************************************************************************************\
//  * File command execute support
// \*********************************************************************************************/

// bool mFileSystem::UfsExecuteCommandFileReady(void) {
//   return (UfsData.run_file_pos < 0);   // Check file ready to disable concurrency
// }

// void mFileSystem::UfsExecuteCommandFileLoop(void) {
//   if (UfsExecuteCommandFileReady() || !ffs_type) { return; }

// //   if (TimeReached(TasmotaGlobal.backlog_timer) && strlen(UfsData.run_file) && !UfsData.run_file_mutex) {
// //     File file = ffsp->open(UfsData.run_file, "r");
// //     if (!file || !file.seek(UfsData.run_file_pos)) {
// //       UfsData.run_file_pos = -1;       // Signal file ready
// //       return;
// //     }

// //     UfsData.run_file_mutex = true;

// //     char cmd_line[512];
// //     cmd_line[0] = '\0';                // Clear in case of re-entry
// //     while (file.available()) {
// //       uint16_t index = 0;
// //       bool comment = false;
// //       while (file.available()) {
// //         uint8_t buf[1];
// //         file.read(buf, 1);
// //         if ((buf[0] == '\n') || (buf[0] == '\r')) {
// //           break;                       // End of command with linefeed or carriage return
// //         }
// //         else if (index && !comment && (buf[0] == ';')) {
// //           break;                       // End of command on multi command line
// //         }
// //         else if ((0 == index) && isspace(buf[0])) {
// //                                        // Skip leading spaces (' ','\t','\n','\v','\f','\r')
// //         }
// //         else if ((0 == index) && (buf[0] == ';')) {
// //           comment = true;              // Ignore comment lines until linefeed or carriage return
// //         }
// //         else if (!comment && (index < sizeof(cmd_line) - 2)) {
// //           cmd_line[index++] = buf[0];  // Build command
// //         }
// //       }
// //       if ((index > 0) && (index < sizeof(cmd_line) - 1)) {
// //         cmd_line[index] = '\0';        // Valid command received
// //         break;
// //       }
// //     }
// //     UfsData.run_file_pos = (file.available()) ? file.position() : -1;
// //     file.close();
// //     // if (strlen(cmd_line)) {
// //     //   bool nodelay = (!(!strncasecmp_P(cmd_line, PSTR(D_CMND_DELAY), strlen(D_CMND_DELAY))));
// //     //   ExecuteCommand(cmd_line, SRC_FILE);
// //     //   if (nodelay) {
// //     //     TasmotaGlobal.backlog_timer = millis();  // Reset backlog_timer which has been set by ExecuteCommand (CommandHandler)
// //     //   }
// //     // }

// //     UfsData.run_file_mutex = false;
// //   }
// }

// bool mFileSystem::UfsExecuteCommandFile(const char *fname) {
//   // Check for non-concurrency and file existance
//   if (UfsExecuteCommandFileReady() && TfsFileExists(fname)) {
//     snprintf(UfsData.run_file, sizeof(UfsData.run_file), fname);
//     UfsData.run_file_pos = 0;          // Signal start of file
//     return true;
//   }
//   return false;
// }

// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/

// const int UFS_FILENAME_SIZE = 48;

// char* mFileSystem::UfsFilename(char* fname, char* fname_in) {
//   fname_in = Trim(fname_in);  // Remove possible leading spaces
//   snprintf_P(fname, UFS_FILENAME_SIZE, PSTR("%s%s"), ('/' == fname_in[0]) ? "" : "/", fname_in);
//   return fname;
// }

// // const char kUFSCommands[] PROGMEM = "Ufs|"  // Prefix
// //   "|Type|Size|Free|Delete|Rename|Run";

// // void (* const kUFSCommand[])(void) PROGMEM = {
// //   &UFSInfo, &UFSType, &UFSSize, &UFSFree, &UFSDelete, &UFSRename, &UFSRun};

// void mFileSystem::UFSInfo(void) {
// //   Response_P(PSTR("{\"Ufs\":{\"Type\":%d,\"Size\":%d,\"Free\":%d}"), ufs_type, UfsInfo(0, 0), UfsInfo(1, 0));
// //   if (ffs_type && (ffs_type != ufs_type)) {
// //     ResponseAppend_P(PSTR(",{\"Type\":%d,\"Size\":%d,\"Free\":%d}"), ffs_type, UfsInfo(0, 1), UfsInfo(1, 1));
// //   }
// //   ResponseJsonEnd();
// }

// void mFileSystem::UFSType(void) {
// //   if (ffs_type && (ffs_type != ufs_type)) {
// //     Response_P(PSTR("{\"%s\":[%d,%d]}"), XdrvMailbox.command, ufs_type, ffs_type);
// //   } else {
// //     ResponseCmndNumber(ufs_type);
// //   }
// }

// void mFileSystem::UFSSize(void) {
// //   if (ffs_type && (ffs_type != ufs_type)) {
// //     Response_P(PSTR("{\"%s\":[%d,%d]}"), XdrvMailbox.command, UfsInfo(0, 0), UfsInfo(0, 1));
// //   } else {
// //     ResponseCmndNumber(UfsInfo(0, 0));
// //   }
// }

// void mFileSystem::UFSFree(void) {
// //   if (ffs_type && (ffs_type != ufs_type)) {
// //     Response_P(PSTR("{\"%s\":[%d,%d]}"), XdrvMailbox.command, UfsInfo(1, 0), UfsInfo(1, 1));
// //   } else {
// //     ResponseCmndNumber(UfsInfo(1, 0));
// //   }
// }

// void mFileSystem::UFSDelete(void) {
//   // UfsDelete  sdcard or flashfs file if only one of them available
//   // UfsDelete2 flashfs file if available
// //   if (XdrvMailbox.data_len > 0) {
// //     char fname[UFS_FILENAME_SIZE];
// //     UfsFilename(fname, XdrvMailbox.data);
// //     bool result = false;
// //     if (ffs_type && (ffs_type != ufs_type) && (2 == XdrvMailbox.index)) {
// //       result = TfsDeleteFile(fname);
// //     } else {
// //       result = (ufs_type && ufsp->remove(fname));
// //     }
// //     if (!result) {
// //     //   ResponseCmndFailed();
// //     } else {
// //     //   ResponseCmndDone();
// //     }
// //   }
// }

// void mFileSystem::UFSRename(void) {
//   // UfsRename  sdcard or flashfs file if only one of them available
//   // UfsRename2 flashfs file if available
// //   if (XdrvMailbox.data_len > 0) {
// //     bool result = false;
// //     char *fname1 = strtok(XdrvMailbox.data, ",");
// //     char *fname2 = strtok(nullptr, ",");
// //     if (fname1 && fname2) {
// //       char fname_old[UFS_FILENAME_SIZE];
// //       UfsFilename(fname_old, fname1);
// //       char fname_new[UFS_FILENAME_SIZE];
// //       UfsFilename(fname_new, fname2);
// //       if (ffs_type && (ffs_type != ufs_type) && (2 == XdrvMailbox.index)) {
// //         result = TfsRenameFile(fname_old, fname_new);
// //       } else {
// //         result = (ufs_type && ufsp->rename(fname_old, fname_new));
// //       }
// //     }
// //     if (!result) {
// //       ResponseCmndFailed();
// //     } else {
// //       ResponseCmndDone();
// //     }
// //   }
// }

// void mFileSystem::UFSRun(void) {
// //   if (XdrvMailbox.data_len > 0) {
// //     char fname[UFS_FILENAME_SIZE];
// //     if (UfsExecuteCommandFile(UfsFilename(fname, XdrvMailbox.data))) {
// //       ResponseClear();
// //     } else {
// //       ResponseCmndFailed();
// //     }
// //   } else {
// //     bool not_active = UfsExecuteCommandFileReady();
// //     UfsData.run_file_pos = -1;
// //     ResponseCmndChar(not_active ? PSTR(D_JSON_DONE) : PSTR(D_JSON_ABORTED));
// //   }
// }

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

// bool Xdrv50(uint8_t function) {
//   bool result = false;

//   switch (function) {
   
//   }
//   return result;
// }

// void mFileSystem::listDir(fs::FS &fs, const char * dirname, uint8_t levels){
//     Serial.printf("Listing directory: %s\n", dirname);

//     File root = fs.open(dirname);
//     if(!root){
//         Serial.println("Failed to open directory");
//         return;
//     }
//     if(!root.isDirectory()){
//         Serial.println("Not a directory");
//         return;
//     }

//     File file = root.openNextFile();
//     while(file){
//         if(file.isDirectory()){
//             Serial.print("  DIR : ");
//             Serial.println(file.name());
//             if(levels){
//                 listDir(fs, file.name(), levels -1);
//             }
//         } else {
//             Serial.print("  FILE: ");
//             Serial.print(file.name());
//             Serial.print("  SIZE: ");
//             Serial.println(file.size());
//         }
//         file = root.openNextFile();
//     }
// }

// void mFileSystem::createDir(fs::FS &fs, const char * path){
//     Serial.printf("Creating Dir: %s\n", path);
//     if(fs.mkdir(path)){
//         Serial.println("Dir created");
//     } else {
//         Serial.println("mkdir failed");
//     }
// }

// void mFileSystem::removeDir(fs::FS &fs, const char * path){
//     Serial.printf("Removing Dir: %s\n", path);
//     if(fs.rmdir(path)){
//         Serial.println("Dir removed");
//     } else {
//         Serial.println("rmdir failed");
//     }
// }

// void mFileSystem::readFile(fs::FS &fs, const char * path){
//     Serial.printf("Reading file: %s\n", path);

//     File file = fs.open(path);
//     if(!file){
//         Serial.println("Failed to open file for reading");
//         return;
//     }

//     Serial.print("Read from file: ");
//     while(file.available()){
//         Serial.write(file.read());
//     }
// }

// void mFileSystem::writeFile(fs::FS &fs, const char * path, const char * message){
//     Serial.printf("Writing file: %s\n", path);

//     File file = fs.open(path, FILE_WRITE);
//     if(!file){
//         Serial.println("Failed to open file for writing");
//         return;
//     }
//     if(file.print(message)){
//         Serial.println("File written");
//     } else {
//         Serial.println("Write failed");
//     }
// }

// void mFileSystem::appendFile(fs::FS &fs, const char * path, const char * message){
//     Serial.printf("Appending to file: %s\n", path);

//     File file = fs.open(path, FILE_APPEND);
//     if(!file){
//         Serial.println("Failed to open file for appending");
//         return;
//     }
//     if(file.print(message)){
//         Serial.println("Message appended");
//     } else {
//         Serial.println("Append failed");
//     }
// }

// void mFileSystem::renameFile(fs::FS &fs, const char * path1, const char * path2){
//     Serial.printf("Renaming file %s to %s\n", path1, path2);
//     if (fs.rename(path1, path2)) {
//         Serial.println("File renamed");
//     } else {
//         Serial.println("Rename failed");
//     }
// }

// void mFileSystem::deleteFile(fs::FS &fs, const char * path){
//     Serial.printf("Deleting file: %s\n", path);
//     if(fs.remove(path)){
//         Serial.println("File deleted");
//     } else {
//         Serial.println("Delete failed");
//     }
// }

// void mFileSystem::testFileIO(fs::FS &fs, const char * path){
//     File file = fs.open(path);
//     static uint8_t buf[512];
//     size_t len = 0;
//     uint32_t start = millis();
//     uint32_t end = start;
//     if(file){
//         len = file.size();
//         size_t flen = len;
//         start = millis();
//         while(len){
//             size_t toRead = len;
//             if(toRead > 512){
//                 toRead = 512;
//             }
//             file.read(buf, toRead);
//             len -= toRead;
//         }
//         end = millis() - start;
//         Serial.printf("%u bytes read for %u ms\n", flen, end);
//         file.close();
//     } else {
//         Serial.println("Failed to open file for reading");
//     }


//     file = fs.open(path, FILE_WRITE);
//     if(!file){
//         Serial.println("Failed to open file for writing");
//         return;
//     }

//     size_t i;
//     start = millis();
//     for(i=0; i<2048; i++){
//         file.write(buf, 512);
//     }
//     end = millis() - start;
//     Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
//     file.close();
// }

// // void setup(){
// //     Serial.begin(115200);
// // }

// // void loop(){

// //   Serial.println(millis());

// // }



// /*********************************************************************************************\
//   Sonoff iFan02 and iFan03
// \*********************************************************************************************/

// /*********************************************************************************************/

// // bool mFileSystem::IsModuleIfan(void)
// // {
// //   return ((SONOFF_IFAN02 == my_module_type) || (SONOFF_IFAN03 == my_module_type));
// // }

// // uint8_t mFileSystem::MaxFanspeed(void)
// // {
// //   return MAX_FAN_SPEED;
// // }

// // uint8_t mFileSystem::GetFanspeed(void)
// // {
// //   if (ifan_fanspeed_timer) {
// //     return ifan_fanspeed_goal;                     // Do not show sequence fanspeed
// //   } else {
// //     /* Fanspeed is controlled by relay 2, 3 and 4 as in Sonoff 4CH.
// //       000x = 0
// //       001x = 1
// //       011x = 2
// //       101x = 3 (ifan02) or 100x = 3 (ifan03)
// //     */
// //     uint8_t fanspeed = (uint8_t)( pCONT_set->power &0xF) >> 1;
// //     if (fanspeed) { fanspeed = (fanspeed >> 1) +1; }  // 0, 1, 2, 3
// //     return fanspeed;
// //   }
// // }


// // // Probably to be handled using "light_interface?"
// // // uint8_t mFileSystem::GetLightState(void)
// // // {
// // //   return pCONT_mry->CommandGet_Relay_Power(0);
// // // }
// // // void mFileSystem::SetLightState(uint8_t state)
// // // {
// // //   pCONT_mry->CommandSet_Relay_Power(state);
// // // }

// // /*********************************************************************************************/

// // void mFileSystem::SetFanSpeed(uint8_t fanspeed, bool sequence)
// // {
// //   ifan_fanspeed_timer = 0;                         // Stop any sequence
// //   ifan_fanspeed_goal = fanspeed;

// //   uint8_t fanspeed_now = GetFanspeed();

// //   // if (fanspeed == fanspeed_now) { return; }

// //   // uint8_t fans;// = kIFan02Speed[fanspeed];
// //   // // if (SONOFF_IFAN03 == my_module_type) {
// //   // //   if (sequence) {
// //   // //     fanspeed = kIFan03Sequence[fanspeed_now][ifan_fanspeed_goal];
// //   // //     if (fanspeed != ifan_fanspeed_goal) {
// //   // //       if (0 == fanspeed_now) {
// //   // //         ifan_fanspeed_timer = 20;                // Need extra time to power up fan
// //   // //       } else {
// //   // //         ifan_fanspeed_timer = 2;
// //   // //       }
// //   // //     }
// //   // //   }
// //   //   fans = kIFan03Speed[fanspeed];
// //   // // }
// //   // for (uint32_t i = 2; i < 5; i++) {
// //   //   uint8_t state = (fans &1) + POWER_OFF_NO_STATE;  // Add no publishPowerState
// //   //   // pCONT_mry->ExecuteCommandPower(i, state, SRC_IGNORE);     // Use relay 2, 3 and 4
// //   //   fans >>= 1;
// //   // }

// // }

// // /*********************************************************************************************/

// // // void mFileSystem::SonoffIfanReceived(void)
// // // {
// // //   char svalue[32];

// // //   uint8_t mode = serial_in_buffer[3];
// // //   uint8_t action = serial_in_buffer[6];

// // //   if (4 == mode) {
// // //     if (action < 4) {
// // //       // AA 55 01 04 00 01 00 06 - Fan 0
// // //       // AA 55 01 04 00 01 01 07 - Fan 1
// // //       // AA 55 01 04 00 01 02 08 - Fan 2
// // //       // AA 55 01 04 00 01 03 09 - Fan 3
// // //       if (action != GetFanspeed()) {
// // //         snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_FANSPEED " %d"), action);
// // //         ExecuteCommand(svalue, SRC_REMOTE);
// // // #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
// // //         BuzzerEnabledBeep((action) ? action : 1, (action) ? 1 : 4);  // Beep action times
// // // #endif
// // //       }
// // //     } else {
// // //       // AA 55 01 04 00 01 04 0A - Light
// // //       ExecuteCommandPower(1, POWER_TOGGLE, SRC_REMOTE);
// // //     }
// // //   }
// // //   if (6 == mode) {
// // //     // AA 55 01 06 00 01 01 09 - Buzzer
// // //     Settings.flag3.buzzer_enable = !Settings.flag3.buzzer_enable;  // SetOption67 - Enable buzzer when available
// // //   }
// // //   if (7 == mode) {
// // //     // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
// // // #ifdef USE_MODULE_DRIVERS_BUZZER
// // //     BuzzerEnabledBeep(4, 1);                       // Beep four times
// // // #endif
// // //   }

// // //   // Send Acknowledge - Copy first 5 bytes, reset byte 6 and store crc in byte 7
// // //   // AA 55 01 04 00 00 05
// // //   serial_in_buffer[5] = 0;                      // Ack
// // //   serial_in_buffer[6] = 0;                      // Crc
// // //   for (uint32_t i = 0; i < 7; i++) {
// // //     if ((i > 1) && (i < 6)) { serial_in_buffer[6] += serial_in_buffer[i]; }
// // //     Serial.write(serial_in_buffer[i]);
// // //   }
// // // }

// // // bool mFileSystem::SerialInput(void)
// // // {
// // //   if (SONOFF_IFAN03 == my_module_type) {
// // //     if (0xAA == serial_in_byte) {               // 0xAA - Start of text
// // //       serial_in_byte_counter = 0;
// // //       ifan_receive_flag = true;
// // //     }
// // //     if (ifan_receive_flag) {
// // //       serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
// // //       if (serial_in_byte_counter == 8) {
// // //         // AA 55 01 01 00 01 01 04 - Wifi long press - start wifi setup
// // //         // AA 55 01 01 00 01 02 05 - Rf and Wifi short press
// // //         // AA 55 01 04 00 01 00 06 - Fan 0
// // //         // AA 55 01 04 00 01 01 07 - Fan 1
// // //         // AA 55 01 04 00 01 02 08 - Fan 2
// // //         // AA 55 01 04 00 01 03 09 - Fan 3
// // //         // AA 55 01 04 00 01 04 0A - Light
// // //         // AA 55 01 06 00 01 01 09 - Buzzer
// // //         // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
// // //         AddLogSerial(LOG_LEVEL_DEBUG);
// // //         uint8_t crc = 0;
// // //         for (uint32_t i = 2; i < 7; i++) {
// // //           crc += serial_in_buffer[i];
// // //         }
// // //         if (crc == serial_in_buffer[7]) {
// // //           SonoffIfanReceived();
// // //           ifan_receive_flag = false;
// // //           return true;
// // //         }
// // //       }
// // //       serial_in_byte = 0;
// // //     }
// // //     return false;
// // //   }
// // // }

// // /*********************************************************************************************\
// //  * Commands
// // \*********************************************************************************************/

// // void mFileSystem::CmndFanspeed(void)
// // {
// //   // if (XdrvMailbox.data_len > 0) {
// //   //   if ('-' == XdrvMailbox.data[0]) {
// //   //     XdrvMailbox.payload = (int16_t)GetFanspeed() -1;
// //   //     if (XdrvMailbox.payload < 0) { XdrvMailbox.payload = MAX_FAN_SPEED -1; }
// //   //   }
// //   //   else if ('+' == XdrvMailbox.data[0]) {
// //   //     XdrvMailbox.payload = GetFanspeed() +1;
// //   //     if (XdrvMailbox.payload > MAX_FAN_SPEED -1) { XdrvMailbox.payload = 0; }
// //   //   }
// //   // }
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_FAN_SPEED)) {
// //   //   SetFanSpeed(XdrvMailbox.payload, true);
// //   // }
// //   // ResponseCmndNumber(GetFanspeed());
// // }

// // /*********************************************************************************************/

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


}


void mFileSystem::Pre_Init(){

//change this into the fan module
//   if(pCONT_pins->PinUsed(GPIO_FAN_SDCARD1_ID)) {  // not set when 255
//     pin = pCONT_pins->GetPin(GPIO_FAN_SDCARD1_ID);
//     pinMode(pin, OUTPUT);
//     settings.fEnableModule = true;
//   }

}


int8_t mFileSystem::Tasker(uint8_t function, JsonParserObject obj){

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
    case FUNC_EVERY_SECOND:  
    
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

     case FUNC_LOOP:
    //   UfsExecuteCommandFileLoop();
      break;
#ifdef USE_SDCARD
    case FUNC_PRE_INIT:
      UfsCheckSDCardInit();
      break;
#endif // USE_SDCARD
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
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END Tasker


// int8_t mFileSystem::Tasker(uint8_t function, JsonParserObject obj), JsonObjectConst obj){
//   switch(function){
//     case FUNC_JSON_COMMAND_OBJECT:
//       parse_JSONCommand(obj);
//     break;
//     case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
//       return CheckAndExecute_JSONCommands(obj);
//     break;
//   }
// }
// int8_t mFileSystem::CheckAndExecute_JSONCommands(JsonObjectConst obj){

//   // Check if instruction is for me
//   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/ifan")>=0){
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_HEATING));
//       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//       parse_JSONCommand(obj);
//       return FUNCTION_RESULT_HANDLED_ID;
//   }else{
//     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
//   }

// }



uint8_t mFileSystem::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  // Active rgbcct palette used as scene

  //Serial.println("mFileSystem::ConstructJSON_Settings");

  char buffer[30];
  
  JsonBuilderI->Start();  

  // Got to ConstructJson_Scene out, or rename all the parameters as something else, or rgbcctactivepalette, or show them all? though that would need to run through, can only show
  // active_id, plus the values below
  // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  //   JsonBuilderI->Add_P(PM_JSON_SCENE_NAME, GetSceneName(buffer, sizeof(buffer)));  
  //   #endif //  ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  
    // JsonBuilderI->Add_P(PM_JSON_HUE, rgbcct_controller.getHue360());
    // JsonBuilderI->Add_P(PM_JSON_SAT, rgbcct_controller.getSat255());
    // JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_RGB, rgbcct_controller.getBrightnessRGB255());
    JsonBuilderI->Add_P(PM_JSON_TIME, 1000);
    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}


////////////////////// START OF MQTT /////////////////////////

void mFileSystem::MQTTHandler_Init(){

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mFileSystem::ConstructJSON_Settings;

  
//   ptr = &mqtthandler_sensor_teleperiod;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SCENE_CTR;
//   ptr->ConstructJSON_function = &mFileSystem::ConstructJSON_Scene;

//   ptr = &mqtthandler_sensdebug_teleperiod;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR;
//   ptr->ConstructJSON_function = &mFileSystem::ConstructJSON_Debug;

} //end "MQTTHandler_Init"


void mFileSystem::MQTTHandler_Set_RefreshAll(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  // mqtthandler_animation_teleperiod.flags.SendNow = true;
  // mqtthandler_ambilight_teleperiod.flags.SendNow = true;
//   mqtthandler_scene_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mFileSystem::MQTTHandler_Set_DefaultPeriodRate(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // // mqtthandler_animation_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // // mqtthandler_ambilight_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_scene_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  
} //end "MQTTHandler_Set_DefaultPeriodRate"


void mFileSystem::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID
    //, MQTT_HANDLER_MODULE_SCENE_TELEPERIOD_ID, MQTT_HANDLER_MODULE_DEBUG_PARAMETERS_TELEPERIOD_ID
  };
  
  struct handler<mFileSystem>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod
    //, &mqtthandler_scene_teleperiod, &mqtthandler_debug_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_DRIVERS_FILESYSTEM_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids, 
    sizeof(mqtthandler_list_ids)/sizeof(mqtthandler_list_ids[0]),
    mqtt_handler_id
  );

}


// void mFileSystem::parse_JSONCommand(JsonObjectConst obj){

  

//   if(obj.containsKey(D_JSON_FANSPEED)){
//     int speed = obj[D_JSON_FANSPEED];
//     test_val = speed;
//     if(speed>0){
//       analogWrite(pin, test_val);
//     }else
//     if(speed==1023){
//       digitalWrite(pin, HIGH);      
//     }else{
//       digitalWrite(pin, LOW);
//     }
//     // if(speed>3){
//     //   Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_FANSPEED,D_PARSING_NOMATCH);
//     //   speed=0; //default off
//     // }      
//     // SetFanSpeed(speed, false);
//     // AddLog(LOG_LEVEL_INFO,PSTR("GetFanspeed=%d"),GetFanspeed());
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_FANSPEED,speed);
//     // Response_mP(S_JSON_COMMAND_NVALUE,D_JSON_FANSPEED,speed);
//     // isserviced++;
//   }
  
  

// }




// int8_t mFileSystem::Tasker_Web(uint8_t function){

//   switch(function){
//     case FUNC_WEB_APPEND_ROOT_BUTTONS:{

//       // create command list
//       // char dlist[100]; memset(dlist,0,sizeof(dlist));
//       // pCONT_sup->AppendDList(dlist, D_JSON_LIGHTPOWER);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);

//       uint8_t button_values[5] = {2, 0, 1, 2, 3}; //toggle, fanspeed0-3
          
//       // BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("Fan Controls"),"");

//       char button_value_ctr[10];
//       char button_key_ctr[50];
//       char button_text_ctr[30];

//       // BufferWriterI->Append_P(PSTR("{t}<tr>"));

//       if(settings.fShowManualSlider){
//         BufferWriterI->Append_P(PSTR("<div> Fan Speed </div>"), test_val);
//         BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
//           WEB_HANDLE_SDCARD_SLIDER,               // c - Unique HTML id
//           PSTR("#000"), PSTR("#eee"),   // Black to White
//           4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
//           0, 1023,  // Range 0/1 to 100%
//           test_val,
//           WEB_HANDLE_SDCARD_SLIDER
//         );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));
//       }

//         // for(uint8_t button_id=0;button_id<5;button_id++){
//         //   BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_INSERTS_HANDLE_IHR, 
//         //                             100/(button_id==0?1:4),
//         //                             button_id==0?"4":"", 
//         //                             "buttonh",
//         //                             pCONT_sup->GetTextIndexed_P(button_key_ctr, sizeof(button_key_ctr), button_id, dlist), 
//         //                             pCONT_sup->p_snprintf(button_value_ctr, sizeof(button_value_ctr), "%d", button_values[button_id]),
//         //                             pCONT_sup->GetTextIndexed_P(button_text_ctr, sizeof(button_text_ctr), button_id, kListFanControls),
//         //                             ""
//         //                         );
//         //   // LightPower button gets its own row
//         //   if(button_id==0){ BufferWriterI->Append_P(PSTR("</tr><tr>")); }
//         // }
//       // BufferWriterI->Append_P(PSTR("</tr>{t2}"));

//     }break; 
//     case FUNC_WEB_COMMAND:
//       WebCommand_Parse();      
//     break;
//   }

// }



// void mFileSystem::WebCommand_Parse(void)
// {
//   AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "mRGBAnimator::WebCommand_Parse"));

//   char tmp[100];

//   uint8_t  arg_value = 0;
//  if(pCONT_web->request_web_command == nullptr){
//   Serial.println("nullptr"); 
//   return; 
// }

//   char arg_ctr[30]; memset(arg_ctr,0,sizeof(arg_ctr));
  
//   sprintf_P(arg_ctr,PSTR(WEB_HANDLE_SDCARD_SLIDER));
//   if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     test_val = arg_value;

//     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//       analogWrite(pin, test_val);
//     // AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
//     // SetRefreshLEDs();
//   }

// }



// uint8_t mFileSystem::ConstructJSON_Settings(uint8_t json_level, bool json_appending){
  
//   JsonBuilderI->Start();
//     JsonBuilderI->Add_P("test",0);  
//   JsonBuilderI->End();

// }

// uint8_t mFileSystem::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

//   JsonBuilderI->Start();
//     JsonBuilderI->Add_P(D_JSON_LIGHTPOWER, GetLightState());
//     JsonBuilderI->Add_P(D_JSON_FANSPEED, GetFanspeed());  
//   JsonBuilderI->End();

// }




// /*********************************************************************************************************************************************
// ******** MQTT **************************************************************************************************************************************
// **********************************************************************************************************************************************
// ********************************************************************************************************************************************/

// void mFileSystem::MQTTHandler_Init(){

//   ptr = &mqtthandler_settings_teleperiod;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 600; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   ptr->ConstructJSON_function = &mFileSystem::ConstructJSON_Settings;

//   ptr = &mqtthandler_sensor_teleperiod;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 600; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   ptr->ConstructJSON_function = &mFileSystem::ConstructJSON_Sensor;

//   ptr = &mqtthandler_sensor_ifchanged;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 60; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   ptr->ConstructJSON_function = &mFileSystem::ConstructJSON_Sensor;
  
// } //end "MQTTHandler_Init"


// void mFileSystem::MQTTHandler_Set_RefreshAll(){

//   mqtthandler_settings_teleperiod.flags.SendNow = true;
//   mqtthandler_sensor_ifchanged.flags.SendNow = true;
//   mqtthandler_sensor_teleperiod.flags.SendNow = true;

// } //end "MQTTHandler_Init"


// void mFileSystem::MQTTHandler_Set_DefaultPeriodRate(){

//   // mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   // mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

// } //end "MQTTHandler_Set_DefaultPeriodRate"


// void mFileSystem::MQTTHandler_Sender(uint8_t mqtt_handler_id){

//   uint8_t flag_handle_all = false, handler_found = false
//   if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

//   // change switch to use array of pointers?
//   do{

//     switch(mqtt_handler_id){
//       case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; ptr=&mqtthandler_settings_teleperiod; break;
//       case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; ptr=&mqtthandler_sensor_ifchanged; break;
//       case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; ptr=&mqtthandler_sensor_teleperiod; break;
//       default: handler_found=false; break; // nothing 
//     } // switch

//     // Pass handlers into command to test and (ifneeded) execute
//     if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_DRIVERS_SDCARD_ID,ptr); }

//     // stop searching
//     if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

//   }while(flag_handle_all);

// }

////////////////////// END OF MQTT /////////////////////////


/******************************************************************************************************************
 * mFileSystem_Commands.cpp
*******************************************************************************************************************/

// #include "mSDCard.h"

// #ifdef USE_MODULE_DRIVERS_SDCARD


// int8_t mSDCard::CheckAndExecute_JSONCommands(){

//   // Check if instruction is for me
//   if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_CONTROLLER_FAN_FRIENDLY_CTR)>=0){
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_TOPIC_COMMAND D_MODULE_CONTROLLER_FAN_FRIENDLY_CTR));
//     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//     parse_JSONCommand();
//     return FUNCTION_RESULT_HANDLED_ID;
//   }else{
//     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
//   }

// }

// void mSDCard::parse_JSONCommand(void){

//   char buffer[50];

//   // Need to parse on a copy
//   char parsing_buffer[data_buffer.payload.len+1];
//   memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
//   JsonParser parser(parsing_buffer);
//   JsonParserObject obj = parser.getRootObject();   
//   if (!obj) { 
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_ERROR, PSTR(D_JSON_DESERIALIZATION_ERROR));
//     #endif //ENABLE_LOG_LEVEL_COMMANDS
//     return;
//   }  
//   JsonParserToken jtok = 0; 
//   int8_t tmp_id = 0;
    

//   if(jtok = obj["ListDir"]){

//     CommandSet_SerialPrint_FileNames(jtok.getStr());
    
//   }
  

//   if(jtok = obj["WriteFile"]){

//     // Also check for datafile
//     JsonParserToken jtok_data = obj["DataFile"];
//     if(!jtok_data.isNull()){
//       CommandSet_WriteFile(jtok.getStr(), jtok_data.getStr());
//     }
//     else{
//       CommandSet_WriteFile(jtok.getStr());
//     }


//   //   if(jtok.isStr()){
//   //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
//   //       CommandSet_PaletteID(tmp_id);
//   //       data_buffer.isserviced++;
//   //     }
//   //   }else
//     // if(jtok.isNum()){
//     //   CommandSet_FanSpeed_Manual(map(jtok.getInt(),0,3,1,1023)); //fix
//     //   set_fan_pwm = map(jtok.getInt(),0,3,1,1023);
//     //   set_fan_speed = jtok.getInt();
//     //   data_buffer.isserviced++;
//     // }
//   //   #ifdef ENABLE_LOG_LEVEL_DEBUG
//   //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
//   //   #endif // ENABLE_LOG_LEVEL_DEBUG
//   }
  
//   if(jtok = obj["ReadFile"]){

//     CommandSet_ReadFile(jtok.getStr());
    
//   }
  
//   mqtthandler_sensor_ifchanged.flags.SendNow = true;

// }


// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** CommandSet_ReadFile *****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mSDCard::CommandSet_ReadFile(const char* filename){

//   readFile(SD_MMC, filename);

//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("ReadFile")), filename);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// } 


// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** CommandSet_WriteFile *****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mSDCard::CommandSet_WriteFile(const char* filename, const char* data){

//   char filename_with_extention[50];
//   sprintf(filename_with_extention, "%s.txt", filename);
//   if(data == nullptr){
//     writeFile(SD_MMC, filename_with_extention, "Empty File!");
//   }else{
//     writeFile(SD_MMC, filename_with_extention, "Hello ");
//   }

//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("WriteFile")), filename_with_extention);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// } 


// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** PixelHardwareType *****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mSDCard::CommandSet_CreateFile_WithName(char* value){

// //   analogWrite(pin, value);
//   createDir(SD_MMC, "/mydir");

// // AddLog(LOG_LEVEL_TEST,PSTR("pwm %d value = %d"),pin,value);


//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("ListDir")), dirname);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// } 

// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** PixelHardwareType *****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mSDCard::CommandSet_SerialPrint_FileNames(const char* dirname){

//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("TESTListDir")), dirname);
//   listDir(SD_MMC, dirname, 0);

//   // listDir(SD_MMC, "/", 0);

//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("ListDir")), dirname);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// } 

// #endif // USE_MODULE_LIGHTS_INTERFACE



/******************************************************************************************************************
 * mFileSystem_WebServer.cpp
*******************************************************************************************************************/


/*********************************************************************************************\
 * Web support
\*********************************************************************************************/

#ifdef USE_WEBSERVER

const char UFS_WEB_DIR[] PROGMEM =
  "<p><form action='" "ufsd" "' method='get'><button>" "%s" "</button></form></p>";

const char UFS_FORM_FILE_UPLOAD[] PROGMEM =
  "<div id='f1' name='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;" D_MANAGE_FILE_SYSTEM "&nbsp;</b></legend>";
const char UFS_FORM_FILE_UPGc[] PROGMEM =
  "<div style='text-align:left;color:#%06x;'>" D_FS_SIZE " %s MB - " D_FS_FREE " %s MB";

const char UFS_FORM_FILE_UPGc1[] PROGMEM =
    " &nbsp;&nbsp;<a href='http://%_I/ufsd?dir=%d'>%s</a>";

const char UFS_FORM_FILE_UPGc2[] PROGMEM =
  "</div>";

const char UFS_FORM_FILE_UPG[] PROGMEM =
  "<form method='post' action='ufsu' enctype='multipart/form-data'>"
  "<br><input type='file' name='ufsu'><br>"
  "<br><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>"
  "<br>";
const char UFS_FORM_SDC_DIRa[] PROGMEM =
  "<div style='text-align:left;overflow:auto;height:250px;'>";
const char UFS_FORM_SDC_DIRc[] PROGMEM =
  "</div>";
const char UFS_FORM_FILE_UPGb[] PROGMEM =
  "</fieldset>"
  "</div>"
  "<div id='f2' name='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";
const char UFS_FORM_SDC_DIRd[] PROGMEM =
  "<pre><a href='%s' file='%s'>%s</a></pre>";
const char UFS_FORM_SDC_DIRb[] PROGMEM =
  "<pre><a href='%s' file='%s'>%s</a> %s %8d %s</pre>";
const char UFS_FORM_SDC_HREF[] PROGMEM =
  "http://%_I/ufsd?download=%s/%s";
#ifdef GUI_TRASH_FILE
const char UFS_FORM_SDC_HREFdel[] PROGMEM =
  //"<a href=http://%_I/ufsd?delete=%s/%s>&#128465;</a>";
  "<a href=http://%_I/ufsd?delete=%s/%s>&#128293;</a>"; // 🔥
#endif // GUI_TRASH_FILE

void UfsDirectory(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_MANAGE_FILE_SYSTEM));

  uint8_t depth = 0;

  strcpy(ufs_path, "/");

  if (Webserver->hasArg(F("download"))) {
    String stmp = Webserver->arg(F("download"));
    char *cp = (char*)stmp.c_str();
    if (UfsDownloadFile(cp)) {
      // is directory
      strcpy(ufs_path, cp);
    } else {
      return;
    }
  }

  if (Webserver->hasArg(F("dir"))) {
    String stmp = Webserver->arg(F("dir"));
    ufs_dir = atoi(stmp.c_str());
    if (ufs_dir == 1) {
      dfsp = ufsp;
    } else {
      if (ffsp) {
        dfsp = ffsp;
      }
    }
  }

  if (Webserver->hasArg(F("delete"))) {
    String stmp = Webserver->arg(F("delete"));
    char *cp = (char*)stmp.c_str();
    dfsp->remove(cp);
  }

  WSContentStart_P(PSTR(D_MANAGE_FILE_SYSTEM));
  WSContentSendStyle();
  WSContentSend_P(UFS_FORM_FILE_UPLOAD);

  char ts[FLOATSZ];
  dtostrfd((float)UfsInfo(0, ufs_dir == 2 ? 1:0) / 1000, 3, ts);
  char fs[FLOATSZ];
  dtostrfd((float)UfsInfo(1, ufs_dir == 2 ? 1:0) / 1000, 3, fs);
  WSContentSend_PD(UFS_FORM_FILE_UPGc, WebColor(COL_TEXT), ts, fs);

  if (ufs_dir) {
    WSContentSend_P(UFS_FORM_FILE_UPGc1, (uint32_t)WiFi.localIP(), (ufs_dir == 1)?2:1, (ufs_dir == 1)?PSTR("SDCard"):PSTR("FlashFS"));
  }
  WSContentSend_P(UFS_FORM_FILE_UPGc2);

  WSContentSend_P(UFS_FORM_FILE_UPG, PSTR(D_SCRIPT_UPLOAD));

  WSContentSend_P(UFS_FORM_SDC_DIRa);
  if (ufs_type) {
    UfsListDir(ufs_path, depth);
  }
  WSContentSend_P(UFS_FORM_SDC_DIRc);
  WSContentSend_P(UFS_FORM_FILE_UPGb);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();

  Web.upload_file_type = UPL_UFSFILE;
}

void UfsListDir(char *path, uint8_t depth) {
  char name[32];
  char npath[128];
  char format[12];
  sprintf(format, PSTR("%%-%ds"), 24 - depth);

  File dir = dfsp->open(path, UFS_FILE_READ);
  if (dir) {
    dir.rewindDirectory();
    if (strlen(path)>1) {
      ext_snprintf_P(npath, sizeof(npath), PSTR("http://%_I/ufsd?download=%s"), (uint32_t)WiFi.localIP(), path);
      for (uint32_t cnt = strlen(npath) - 1; cnt > 0; cnt--) {
        if (npath[cnt] == '/') {
          if (npath[cnt - 1] == '=') {
            npath[cnt + 1] = 0;
          } else {
            npath[cnt] = 0;
          }
          break;
        }
      }
      WSContentSend_P(UFS_FORM_SDC_DIRd, npath, path, PSTR(".."));
    }
    char *ep;
    while (true) {
      File entry = dir.openNextFile();
      if (!entry) {
        break;
      }
      // esp32 returns path here, shorten to filename
      ep = (char*)entry.name();
      if (*ep == '/') { ep++; }
      char *lcp = strrchr(ep,'/');
      if (lcp) {
        ep = lcp + 1;
      }

      uint32_t tm = entry.getLastWrite();
      String tstr = GetDT(tm);

      char *pp = path;
      if (!*(pp + 1)) { pp++; }
      char *cp = name;
      // osx formatted disks contain a lot of stuff we dont want
      if (!UfsReject((char*)ep)) {

        for (uint8_t cnt = 0; cnt<depth; cnt++) {
          *cp++ = '-';
        }

        sprintf(cp, format, ep);
        if (entry.isDirectory()) {
          ext_snprintf_P(npath, sizeof(npath), UFS_FORM_SDC_HREF, (uint32_t)WiFi.localIP(), pp, ep);
          WSContentSend_P(UFS_FORM_SDC_DIRd, npath, ep, name);
          uint8_t plen = strlen(path);
          if (plen > 1) {
            strcat(path, "/");
          }
          strcat(path, ep);
          UfsListDir(path, depth + 4);
          path[plen] = 0;
        } else {
#ifdef GUI_TRASH_FILE
          char delpath[128];
          ext_snprintf_P(delpath, sizeof(delpath), UFS_FORM_SDC_HREFdel, (uint32_t)WiFi.localIP(), pp, ep);
#else
          char delpath[2];
          delpath[0]=0;
#endif // GUI_TRASH_FILE
          ext_snprintf_P(npath, sizeof(npath), UFS_FORM_SDC_HREF, (uint32_t)WiFi.localIP(), pp, ep);
          WSContentSend_P(UFS_FORM_SDC_DIRb, npath, ep, name, tstr.c_str(), entry.size(), delpath);
        }
      }
      entry.close();
    }
    dir.close();
  }
}

#ifdef ESP32
#define ESP32_DOWNLOAD_TASK
#endif // ESP32

uint8_t UfsDownloadFile(char *file) {
  File download_file;

  if (!dfsp->exists(file)) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: File not found"));
    return 0;
  }

  download_file = dfsp->open(file, UFS_FILE_READ);
  if (!download_file) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: Could not open file"));
    return 0;
  }

  if (download_file.isDirectory()) {
    download_file.close();
    return 1;
  }

#ifndef ESP32_DOWNLOAD_TASK
  WiFiClient download_Client;
  uint32_t flen = download_file.size();

  download_Client = Webserver->client();
  Webserver->setContentLength(flen);

  char attachment[100];
  char *cp;
  for (uint32_t cnt = strlen(file); cnt >= 0; cnt--) {
    if (file[cnt] == '/') {
      cp = &file[cnt + 1];
      break;
    }
  }
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=%s"), cp);
  Webserver->sendHeader(F("Content-Disposition"), attachment);
  WSSend(200, CT_APP_STREAM, "");

  uint8_t buff[512];
  uint32_t bread;
  // transfer is about 150kb/s
  uint32_t cnt = 0;
  while (download_file.available()) {
    bread = download_file.read(buff, sizeof(buff));
    uint32_t bw = download_Client.write((const char*)buff, bread);
    if (!bw) { break; }
    cnt++;
    if (cnt > 7) {
      cnt = 0;
      //if (glob_script_mem.script_loglevel & 0x80) {
        // this indeed multitasks, but is slower 50 kB/s
      //  loop();
      //}
    }
    delay(0);
    OsWatchLoop();
  }
  download_file.close();
  download_Client.stop();
#endif // ESP32_DOWNLOAD_TASK


#ifdef ESP32_DOWNLOAD_TASK
  download_file.close();

  if (UfsData.download_busy == true) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: Download is busy"));
    return 0;
  }

  UfsData.download_busy = true;
  char *path = (char*)malloc(128);
  strcpy(path,file);
  xTaskCreatePinnedToCore(donload_task, "DT", 6000, (void*)path, 3, NULL, 1);
#endif // ESP32_DOWNLOAD_TASK

  return 0;
}


#ifdef ESP32_DOWNLOAD_TASK
#ifndef DOWNLOAD_SIZE
#define DOWNLOAD_SIZE 4096
#endif // DOWNLOAD_SIZE
void donload_task(void *path) {
  File download_file;
  WiFiClient download_Client;
  char *file = (char*) path;

  download_file = dfsp->open(file, UFS_FILE_READ);
  free(file);

  uint32_t flen = download_file.size();

  download_Client = Webserver->client();
  Webserver->setContentLength(flen);

  char attachment[100];
  char *cp;
  for (uint32_t cnt = strlen(file); cnt >= 0; cnt--) {
    if (file[cnt] == '/') {
      cp = &file[cnt + 1];
      break;
    }
  }
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=%s"), cp);
  Webserver->sendHeader(F("Content-Disposition"), attachment);
  WSSend(200, CT_APP_STREAM, "");

  uint8_t *buff = (uint8_t*)malloc(DOWNLOAD_SIZE);
  if (buff) {
    uint32_t bread;
    while (download_file.available()) {
      bread = download_file.read(buff, DOWNLOAD_SIZE);
      uint32_t bw = download_Client.write((const char*)buff, bread);
      if (!bw) { break; }
    }
    free(buff);
  }
  download_file.close();
  download_Client.stop();
  UfsData.download_busy = false;
  vTaskDelete( NULL );
}
#endif //  ESP32_DOWNLOAD_TASK


bool UfsUploadFileOpen(const char* upload_filename) {
  char npath[48];
  snprintf_P(npath, sizeof(npath), PSTR("%s/%s"), ufs_path, upload_filename);
  dfsp->remove(npath);
  ufs_upload_file = dfsp->open(npath, UFS_FILE_WRITE);
  return (ufs_upload_file);
}

bool UfsUploadFileWrite(uint8_t *upload_buf, size_t current_size) {
  if (ufs_upload_file) {
    ufs_upload_file.write(upload_buf, current_size);
  } else {
    return false;
  }
  return true;
}

void UfsUploadFileClose(void) {
  ufs_upload_file.close();
}

#endif  // USE_WEBSERVER



/******************************************************************************************************************
 * 
*******************************************************************************************************************/


#endif // USE_MODULE_DRIVERS_FILESYSTEM
