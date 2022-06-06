#ifndef _USE_MODULE_DRIVERS_SDCARD_H
#define _USE_MODULE_DRIVERS_SDCARD_H 0.3

#define D_UNIQUE_MODULE_DRIVERS_SDCARD_ID 45

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_SDCARD


#include <string.h>
#include <strings.h>


#include "freertos/ringbuf.h"
// static char tx_item[] = "test_item";
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_intr_alloc.h"


extern uint8_t extern_flag_sdcard_writer_status;


/*** Working
 * SD Card using SPI on ESP32
 * 
 * CS   = GPIO05
 * SCK  = GPIO18
 * MOSI = GPIO23
 * MISO = GPIO19
 * VCC = 5V
 * GND
 * */

// /*
//  * Connect the SD card to the following pins:
//  *
//  * SD Card | ESP32
//  *    D2       12
//  *    D3       13
//  *    CMD      15
//  *    VSS      GND
//  *    VDD      3.3V
//  *    CLK      14
//  *    VSS      GND
//  *    D0       2  (add 1K pull up after flashing)
//  *    D1       4
//  */


// /**
//  *  MISO = D0
//  *  MOSI = DI
//  *  CS = 5?
//  *  





#include "FS.h"
#include "SD.h"
#include "SPI.h"


// extern File stored_file;


DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_FILE_WRITER_CTR) "file_writer";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_WRITE_TIMES_CTR) "debug/writetimes";

// #include "FS.h"
// #include "SD_MMC.h"

class mSDCard :
  public mTaskerInterface
{

  private:
  public:
    mSDCard(){};

    static const char* PM_MODULE_DRIVERS_SDCARD_CTR;
    static const char* PM_MODULE_DRIVERS_SDCARD_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_SDCARD_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS_SDCARD_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_SDCARD_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSDCard);
    };
    #endif

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    // int8_t Tasker(uint8_t function, JsonObjectConst obj);   

    int8_t Tasker_Web(uint8_t function);

    enum TEST_MODE_IDS{
      STANDBY_ID=0,
      WRITE_BYTES_ID,
    };


    struct DEBUG{
      uint16_t bytes_to_write = 0;
      TEST_MODE_IDS test_mode = STANDBY_ID;
      
      struct WRITE_TIMES{
      std::vector<uint32_t> opened;
      std::vector<uint32_t> closed;
        // uint32_t opened;
        // uint32_t closed;
      }write_time;
    }debug;

    
    enum EM_LOGGING_STATE_IDS
    {
      EM_LOGGING_STATE_DISABLED_ID=0,
      EM_LOGGING_STATE_ENABLED_ID,
      EM_LOGGING_STATE_LENGTH
    };

    struct SDCARD_DATA{
      bool isopened = false;
      bool isopened_previous_state = false;

      bool init_success_on_boot = false;
      bool init_error_on_boot = false;

      uint32_t bytes_written_to_file = 0;
      // enable_logging should be in logger class

      // isopened_state
      // isopened_state_

      // // flag which if set, means the sdcard should be written to, if closed before, then start progress to open it
      // uint8_t enable_logging = EM_LOGGING_STATE_DISABLED_ID;
      // uint8_t enable_logging_previous_state = EM_LOGGING_STATE_DISABLED_ID;


    }sdcard_status;

    void CommandSet_SDCard_Appending_File_Method_State(uint8_t state);
        
    void EveryLoop_Handle_Appending_File_Method();
    void Handle_Write_Ringbuffer_Stream_To_SDCard();


    #ifdef USE_SDCARD_RINGBUFFER_STREAM_OUT

    #define RINGBUFFER_HANDLE_SDCARD_WRITER_LENGTH 100000 //huge

    struct STREAM_RINGBUFFER
    {
      bool initialised = false;
      bool flag_data_waiting = false;
      RingbufHandle_t ringbuffer_handle;
      QueueHandle_t event_queue_handle;
      // int ring_buffer_size_tx = (RINGBUFFER_HANDLE_2_LENGTH * 2); //1024*2
      // int ring_buffer_size_rx = (RINGBUFFER_HANDLE_2_LENGTH * 2); //1024*2

    }stream;

    uint16_t GetRingBufferDataAndClear(uint8_t uart_num, char* buffer, uint16_t buflen, char optional_read_until_char, bool flag_clear_buffer_after_read);
    void Stream_AddToBuffer(char* buffer, uint16_t buflen);
    void init_SDCard_StreamOut_RingBuffer();
    uint16_t AppendRingBuffer(char* buffer, uint16_t buflen);

    #endif // USE_SDCARD_RINGBUFFER_STREAM_OUT

    
#ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
void init_SDCARD_is_Serial_Debug_Only();
#endif

void parse_JSONCommand(JsonParserObject obj);

uint16_t test_val = 0;
uint8_t dir = 0;

void init();
void Pre_Init();
int8_t pin = -1;
struct SETTINGS{
  uint8_t fEnableModule = false;
  uint8_t fShowManualSlider = false;
}settings;


struct DEBUG_WRITE_TIMES{

  uint8_t write_test_count = 0;
  uint32_t complete_write_duration = 0;
  uint32_t write_byte_count = 0;

}debug_write_times;

void SDCardSpeedDebug();
void SDCardBulkSpeedTest(uint8_t test_number, uint32_t bytes_to_write);


void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);
void testFileIO(fs::FS &fs, const char * path);


void appendFile_open_and_close(fs::FS &fs, const char * path, const char * message);

void write_append_array(fs::FS &fs, const char * path, uint8_t* buffer, uint16_t buflen);


void CommandSet_CreateFile_WithName(char* value);
void CommandSet_SerialPrint_FileNames(const char* value);
void CommandSet_WriteFile(const char* filename, const char* data = nullptr);
    
void CommandSet_ReadFile(const char* filename);

/**
 * Method A
 * */
enum FILE_STATUS_IDS{
  FILE_STATUS_NOT_STARTED=0,
  // Transitional state, will run once
  FILE_STATUS_OPENING_ID,
  // 
  FILE_STATUS_OPENED_ID,
  // FILE_STATUS_APPEND_ID,
  FILE_STATUS_CLOSING_ID,
  FILE_STATUS_CLOSED_ID
};
struct WRITE{
  FILE_STATUS_IDS status = FILE_STATUS_NOT_STARTED;
  char file_name[50] = {0};
  

}writer_settings;
int close_decounter = -1;
void SubTask_Append_To_Open_File(char* buffer = nullptr, uint16_t buflen = 0);



char stream_out_buffer[2048];






int8_t CheckAndExecute_JSONCommands();
void parse_JSONCommand(void);

uint8_t ConstructJSON_Scene(uint8_t json_method);

void WebCommand_Parse(void);


void WebAppend_Root_Draw_PageTable();
void WebAppend_Root_Status_Table();




    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_FileWriter(uint8_t json_method = 0);
    uint8_t ConstructJSON_Debug_WriteTimes(uint8_t json_method = 0);

  
  #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mSDCard> mqtthandler_settings_teleperiod;
    struct handler<mSDCard> mqtthandler_file_writer;
    struct handler<mSDCard> mqtthandler_debug_write_times;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

    struct handler<mSDCard>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_file_writer,
      &mqtthandler_debug_write_times
    };
  #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif