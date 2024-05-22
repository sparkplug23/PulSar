#ifndef _USE_MODULE_DRIVERS_FILESYSTEM_H
#define _USE_MODULE_DRIVERS_FILESYSTEM_H 0.3

#define D_UNIQUE_MODULE_DRIVERS_FILESYSTEM_ID ((4*1000)+02)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_FILESYSTEM


#include <string.h>
#include <strings.h>


#ifdef ESP8266
  #include <LittleFS.h>
  #include <SPI.h>
  #ifdef USE_SDCARD
    #include <SD.h>
    #include <SDFAT.h>
  #endif  // USE_SDCARD
#endif  // ESP8266
#ifdef ESP32
  #include <LittleFS.h>
  #ifdef USE_SDCARD
    #include <SD.h>
  #endif  // USE_SDCARD
  #include "FFat.h"
  #include "FS.h"
#endif  // ESP32




#define FILE_EXTENSION_JSON ".json"

#define FILE_EXTENSION_BIN ".bin" //release version
// #define FILE_EXTENSION_BIN ".txt"   //debug version so the editor can open the file


/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       12
 *    D3       13
 *    CMD      15
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      14
 *    VSS      GND
 *    D0       2  (add 1K pull up after flashing)
 *    D1       4
 */

// #include "FS.h"
// #include "SD_MMC.h"


#include "1_TaskerManager/mTaskerInterface.h"

class mFileSystem :
  public mTaskerInterface
{

  public:
    mFileSystem(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    // int8_t Tasker(uint8_t function, JsonObjectConst obj);   

    int8_t Tasker_Web(uint8_t function);

    void FileWrite_Test();
    
    void JsonFile_Save__Stored_Module();
    bool JsonFile_Load__Stored_Module();
    void JsonFile_Load__Stored_Module_Or_Default_Template();

    void JsonFile_Save__Stored_Secure();
    void JsonFile_Load__Stored_Secure();
    

    static const char* PM_MODULE_DRIVERS_FILESYSTEM_CTR;
    static const char* PM_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR;
    PGM_P GetModuleName(){         return PM_MODULE_DRIVERS_FILESYSTEM_CTR; }
    PGM_P GetModuleFriendlyName(){ return PM_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_FILESYSTEM_ID; }



void ByteFile_Save(char* filename_With_extension, uint8_t* buffer, uint16_t buflen);
uint32_t ByteFile_Load(char* filename_With_extension, uint8_t* buffer, uint16_t buflen);
void JSONFile_Save(char* filename_With_extension, char* buffer, uint16_t buflen);
void JSONFile_Load(char* filename_With_extension, char* buffer, uint16_t buflen);



void SystemTask__Execute_Module_Data_Save();
    
// saves 80 bytes of flash, makes the UI cleaner for folders containing lots of files.
// disables recursive folder listing in file UI
//#define UFILESYS_NO_RECURSE_GUI

// Enables serving of static files on /fs/
// costs 1844 bytes of flash and 40 bytes of RAM
// probably not useful on esp8266, but useful on esp32
// You could serve a whole webapp from Tas itself.
//#define UFILESYS_STATIC_SERVING

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
FS *ufsp = nullptr;
// Flash file system pointer
FS *ffsp = nullptr;
// Local pointer for file managment
FS *dfsp = nullptr;

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



void UfsInitOnce(void);
void UfsInit(void);



// void UfsCheckSDCardInit(void);
// uint8_t UfsReject(char *name);
// bool TfsFileExists(const char *fname){
// bool TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len);
// bool TfsInitFile(const char *fname, uint32_t len, uint8_t init_value);
// bool TfsLoadFile(const char *fname, uint8_t *buf, uint32_t len);
// bool TfsDeleteFile(const char *fname);
// bool TfsRenameFile(const char *fname1, const char *fname2);
// bool UfsExecuteCommandFileReady(void);
// void UfsExecuteCommandFileLoop(void);
// bool UfsExecuteCommandFile(const char *fname);
// char* UfsFilename(char* fname, char* fname_in);
// void UFSInfo(void);
// void UFSType(void);
// void UFSSize(void);
// void UFSDelete(void);
// void UFSRename(void);
// void UFSRun(void);




uint16_t test_val = 0;
uint8_t dir = 0;

void init();
void Pre_Init();
int8_t pin = -1;
struct SETTINGS{
  uint8_t fEnableModule = false;
  uint8_t fShowManualSlider = false;
}settings;




char *folderOnly(char *fname);
char *fileOnly(char *fname);
void UfsCheckSDCardInit(void);
uint32_t UfsInfo(uint32_t sel, uint32_t type);
uint32_t UfsSize(void);
uint32_t UfsFree(void);
uint8_t UfsReject(char *name);
bool TfsFileExists(const char *fname);
size_t TfsFileSize(const char *fname);
bool TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len);
bool TfsInitFile(const char *fname, uint32_t len, uint8_t init_value);
bool TfsLoadFile(const char *fname, uint8_t *buf, uint32_t len);
String TfsLoadString(const char *fname);
bool TfsDeleteFile(const char *fname);
bool TfsRenameFile(const char *fname1, const char *fname2);
bool UfsExecuteCommandFileReady(void);
void UfsExecuteCommandFileLoop(void);
bool UfsExecuteCommandFile(const char *fname);
char* UfsFilename(char* fname, char* fname_in);
void UFSInfo(void);
void UFSType(void);
void UFSSize(void);
void UFSFree(void);
void UFSDelete(void);
void UFSRename(void);




























void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);
void testFileIO(fs::FS &fs, const char * path);


void CommandSet_CreateFile_WithName(char* value);
void CommandSet_SerialPrint_FileNames(const char* value);
void CommandSet_WriteFile(const char* filename, const char* data = nullptr);
    
void CommandSet_ReadFile(const char* filename);



int8_t CheckAndExecute_JSONCommands();
void parse_JSONCommand(JsonParserObject obj);

uint8_t ConstructJSON_Scene(uint8_t json_level, bool json_appending);

void WebCommand_Parse(void);


void WebAppend_Root_Draw_PageTable();
void WebAppend_Root_Status_Table();


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    struct handler<mFileSystem>* ptr;
    void MQTTHandler_Sender();

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mFileSystem> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mFileSystem> mqtthandler_sensor_ifchanged;
    struct handler<mFileSystem> mqtthandler_sensor_teleperiod;
    
    std::vector<struct handler<mFileSystem>*> mqtthandler_list;

};

#endif

#endif