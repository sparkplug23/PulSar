#ifndef _MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_H
#define _MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_H

#define D_UNIQUE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_ID   141  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_ID    1    // Numerical accesending order of module within a group

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER

// #include "5_Sensors/ADCInternal/mADCInternal.h"

//https://i.stack.imgur.com/6pPh9.png

// extern bool extern_flag_sd_stream_buffer_lock ;

#include <driver/adc.h>


// extern struct STREAM_TMP_BUFFERS
// {
  // extern char stream_tmp_buffer_0[1000];
  // extern uint16_t stream_tmp_buflen_0;
  // extern char stream_tmp_buffer_1[1000];
  // extern uint16_t stream_tmp_buflen_1;
  // extern uint8_t stream_tmp_buf_writer_index;
// };
//stream_tmp_buffers[4];



// extern 
// static SemaphoreHandle_t mutex_filling_sd_writer;

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SDCARD_SUPERFRAME_CTR) "sdcard_superframe";

    void ISR_Direct_SubTask_Generate_SyncFrame_To_SDCard_Stream();
    
class mSerialCalibrationMeasurmentLogger :
  public mTaskerInterface
{
  public:
	  mSerialCalibrationMeasurmentLogger(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_CTR;
    static const char* PM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSerialCalibrationMeasurmentLogger);
    };
    #endif
    

    typedef enum 
    {
      ADC_CHANNEL_GROUP_1_ID,
      ADC_CHANNEL_GROUP_2_ID,
      ADC_CHANNEL_GROUP_UNSET_ID
    }adc_channel_group;

    typedef enum
    {
      ADC_MODE_DISABLED_ID=0,
      ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID,
      ADC_MODE_LENGTH_ID,
    }adc_mode;

    #define ADC_CHANNELS_MAX 2
    struct ADC_CONFIG{
      uint8_t input_pin = 0;
      adc_channel_t channel_id = ADC_CHANNEL_0;
      adc_channel_group channel_group =  ADC_CHANNEL_GROUP_UNSET_ID;    
      adc_atten_t attentuation_db_level = ADC_ATTEN_DB_11;
      adc_bits_width_t adc_width_bit = ADC_WIDTH_BIT_12;
      adc_mode mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
    }adc_config[ADC_CHANNELS_MAX];

    struct ADCVALUES{
      struct ESP32V{
        uint16_t adc2;
        uint16_t adc5;
      }esp32;
      struct PIC32V{
        uint16_t adc2;
        uint16_t adc5;
      }pic32;
    }adc_values;


    
    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t fSensorCount= 0; 
    }settings;

    
    uint32_t rxon_counter = 0;

    void UpdateInternalRTCTimeWithGPSTime();


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
    void SubTask_Generate_SyncFrame_To_SDCard_Stream();
    void Construct_SuperFrame_Data_From_RingBuffer();

    
    void SubTask_UpdateOLED();
    
    void SubTask_HandleSDCardLogging();
    void SubTask_Debug_BasicFileWriteTest();
    
    void CommandSet_SDCard_OpenClose_Toggle();
    void CommandSet_LoggingState(uint8_t state);
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
    uint8_t ConstructJSON_SDCard_Calibration_Frame(uint8_t json_method = 0);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mSerialCalibrationMeasurmentLogger> mqtthandler_settings_teleperiod;
    struct handler<mSerialCalibrationMeasurmentLogger> mqtthandler_sensor_ifchanged;
    struct handler<mSerialCalibrationMeasurmentLogger> mqtthandler_sensor_teleperiod;
    struct handler<mSerialCalibrationMeasurmentLogger> mqtthandler_sdcard_superframe;

    //No extra handlers example
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    //with extra handlers example
        
    struct handler<mSerialCalibrationMeasurmentLogger>* mqtthandler_list[4] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod,
      &mqtthandler_sdcard_superframe
    };
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
