#ifndef _MODULE_CONTROLLER_SDCARDLOGGER_H
#define _MODULE_CONTROLLER_SDCARDLOGGER_H

#define D_UNIQUE_MODULE_CONTROLLER_SDCARDLOGGER_ID   9020 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_UARTLOGGER

class mUARTLogger :
  public mTaskerInterface
{
  public:
	  mUARTLogger(){};
    void Pre_Init(void);
    void Init(void);
    
    static constexpr const char* PM_MODULE_CONTROLLER_SDCARDLOGGER_CTR = D_MODULE_CONTROLLER_SDCARDLOGGER_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_SDCARDLOGGER_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_SDCARDLOGGER_ID; }

    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    
    void EverySecond();

    struct SDCARD_DATA{
      bool isopened = false;


    }sdcard_status;
    
    void SubTask_UpdateOLED();
    
    void CommandSet_SDCard_OpenClose_Toggle();
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();
    struct handler<mUARTLogger> mqtthandler_settings;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mUARTLogger> mqtthandler_sensor_ifchanged;
    struct handler<mUARTLogger> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    
    //with extra handlers example
    
    uint8_t list_ids[3] = {
      MQTT_HANDLER_SETTINGS_ID, 
      MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
      MQTT_HANDLER_SENSOR_TELEPERIOD_ID
    };
    
    struct handler<mUARTLogger>* list_ptr[3] = {
      &mqtthandler_settings,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
