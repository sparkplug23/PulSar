#ifndef _USE_MODULE_DRIVERS_FILESYSTEM_H
#define _USE_MODULE_DRIVERS_FILESYSTEM_H 0.3

#define D_UNIQUE_MODULE_DRIVERS_FILESYSTEM_ID 53

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
  #include <LITTLEFS.h>
  #ifdef USE_SDCARD
    #include <SD.h>
  #endif  // USE_SDCARD
  #include "FFat.h"
  #include "FS.h"
#endif  // ESP32

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
    int8_t Tasker(uint8_t function);
    // int8_t Tasker(uint8_t function, JsonObjectConst obj);   

    int8_t Tasker_Web(uint8_t function);

    

    static const char* PM_MODULE_DRIVERS_FILESYSTEM_CTR;
    static const char* PM_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR;
    PGM_P GetModuleName(){         return PM_MODULE_DRIVERS_FILESYSTEM_CTR; }
    PGM_P GetModuleFriendlyName(){ return PM_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_FILESYSTEM_ID; }

    
void UfsInitOnce(void);
void UfsInit(void);
// void UfsCheckSDCardInit(void);
uint32_t UfsInfo(uint32_t sel, uint32_t type);
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
void pre_init();
int8_t pin = -1;
struct SETTINGS{
  uint8_t fEnableModule = false;
  uint8_t fShowManualSlider = false;
}settings;



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
void parse_JSONCommand(void);

uint8_t ConstructJSON_Scene(uint8_t json_method);

void WebCommand_Parse(void);


void WebAppend_Root_Draw_PageTable();
void WebAppend_Root_Status_Table();


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mFileSystem>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mFileSystem> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mFileSystem> mqtthandler_sensor_ifchanged;
    struct handler<mFileSystem> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif