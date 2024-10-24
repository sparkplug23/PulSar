#ifndef _DRIVERS__CAMERA_ARDUINO_H1
#define _DRIVERS__CAMERA_ARDUINO_H1

#define D_UNIQUE_MODULE_DRIVERS__CAMERA_ARDUINO__ID 4060 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS__CAMERA_ARDUINO

#include "1_TaskerManager/mTaskerInterface.h"


#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"


class mWebCamera :
  public mTaskerInterface
{
  public:
    mWebCamera(){};

    static constexpr const char* PM_MODULE_DRIVERS__CAMERA_ARDUINO__CTR = D_MODULE_DRIVERS__CAMERA_ARDUINO__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS__CAMERA_ARDUINO__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS__CAMERA_ARDUINO__ID; }

    struct SETTINGS
    { 
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    void Pre_Init();
    void Init();
    void EverySecond();
    void parse_JSONCommand(JsonParserObject obj);
    
    // void startCameraServer();
    void setupLedFlash(int pin);


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    void MQTTHandler_Sender();
    
    struct handler<mWebCamera> mqtthandler_settings;
    struct handler<mWebCamera> mqtthandler_sensor_ifchanged;
    struct handler<mWebCamera> mqtthandler_sensor_teleperiod;
 
    struct handler<mWebCamera>* mqtthandler_list[3] = {
      &mqtthandler_settings,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    // No specialised payload therefore use system default instead of enum
    
    
    #endif // USE_MODULE_NETWORK_MQTT
    
};

#endif // USE_MODULE_DRIVERS__CAMERA_ARDUINO

#endif // _DRIVERS__CAMERA_ARDUINO_H
