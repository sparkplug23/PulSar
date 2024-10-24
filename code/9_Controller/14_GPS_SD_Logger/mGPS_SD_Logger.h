#ifndef _MODULE_CONTROLLER_GPS_SD_LOGGER_H
#define _MODULE_CONTROLLER_GPS_SD_LOGGER_H

#define D_UNIQUE_MODULE_CONTROLLER_GPS_SD_LOGGER_ID   9014 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER

#include "5_Sensors/ADCInternal/mADCInternal.h"

//https://i.stack.imgur.com/6pPh9.png

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SDCARD_SUPERFRAME_CTR) "sdcard_superframe";

class mGPS_SD_Logger :
  public mTaskerInterface
{
  public:
	  mGPS_SD_Logger(){};
    void Pre_Init(void);
    void Init(void);
    
    static constexpr const char* PM_MODULE_CONTROLLER_GPS_SD_LOGGER_CTR = D_MODULE_CONTROLLER_GPS_SD_LOGGER_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_GPS_SD_LOGGER_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_GPS_SD_LOGGER_ID; }

    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    uint32_t rxon_counter = 0;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    
    void EverySecond();

    uint32_t sequence_test = 0;

    uint32_t tSaved_MillisWrite = 0;
    uint32_t tSaved_MillisWrite2 = 0;

    
    std::vector<uint16_t> readings_rss2;
    std::vector<uint16_t> readings;


    
    struct SDCARD_DATA{
      bool isopened = false;

      // enable_logging should be in logger class

      // isopened_state
      // isopened_state_

      // flag which if set, means the sdcard should be written to, if closed before, then start progress to open it
      uint8_t enable = 0;
      uint8_t enable_previous_state = 0;



    }logger_status;

    #define RSS_RINGBUFFER_NUMBER_INDEX 2
    #define SYNC_FRAME_DATA_BUFFER_LENGTH 5000
    struct SYNC_FRAME_DATA
    {
      // uint8_t trigger_pin = 0;
      bool flag_pin_active = false;

      /**
       * ACTIVE_LOW
       * Low = started
       * High = ended
       * */
      bool flag_started = false;
      bool flag_ended = false;
      char buffer[SYNC_FRAME_DATA_BUFFER_LENGTH];
      uint16_t buflen = 0;
      uint16_t buffer_bytes_read = 0;
    }sync_frame_data;

    uint32_t tSaved_Constructed_Json_SDCardSuperFrame_ms = 0;

    void Handle_Primary_Service_RSS_Stream_To_Create_SDCard_Stream();
    void SubTask_Generate_GPS_Json_SDCard_Stream();
    void Construct_SuperFrame_Data_From_RingBuffer();

    
    void SubTask_UpdateOLED();
    void SubTask_UpdateOLED_Detailed_GPS();
    
    void SubTask_HandleSDCardLogging();
    void SubTask_Debug_BasicFileWriteTest();
    
    void CommandSet_SDCard_OpenClose_Toggle();
    void CommandSet_LoggingState(uint8_t state);
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_SDCardSuperFrame(uint8_t json_level = 0, bool json_appending = true);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();
    struct handler<mGPS_SD_Logger> mqtthandler_settings;
    struct handler<mGPS_SD_Logger> mqtthandler_sensor_ifchanged;
    struct handler<mGPS_SD_Logger> mqtthandler_sensor_teleperiod;
    struct handler<mGPS_SD_Logger> mqtthandler_sdcard_superframe;

    //No extra handlers example
    
    //with extra handlers example
        
    struct handler<mGPS_SD_Logger>* mqtthandler_list[4] = {
      &mqtthandler_settings,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod,
      &mqtthandler_sdcard_superframe
    };
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
