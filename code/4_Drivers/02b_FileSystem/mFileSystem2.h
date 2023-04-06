#ifndef _USE_MODULE_DRIVERS_FILESYSTEM2_H
#define _USE_MODULE_DRIVERS_FILESYSTEM2_H 0.3

#define D_UNIQUE_MODULE_DRIVERS_FILESYSTEM2_ID ((4*1000)+02)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_FILESYSTEM2


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
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    // int8_t Tasker(uint8_t function, JsonObjectConst obj);   

    int8_t Tasker_Web(uint8_t function);

    

    static const char* PM_MODULE_DRIVERS_FILESYSTEM_CTR;
    static const char* PM_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR;
    PGM_P GetModuleName(){         return PM_MODULE_DRIVERS_FILESYSTEM_CTR; }
    PGM_P GetModuleFriendlyName(){ return PM_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_FILESYSTEM_ID; }


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    struct handler<mFileSystem>* ptr;
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