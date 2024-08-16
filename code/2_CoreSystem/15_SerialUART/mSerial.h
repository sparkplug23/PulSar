#ifndef _USE_MODULE_CORE__SERIAL__H
#define _USE_MODULE_CORE__SERIAL__H 0.3

#define D_UNIQUE_MODULE_CORE__SERIAL__ID 2015 /**[(FolderIndex*1000)+ModuleIndex]*/

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE__SERIAL

#include "1_TaskerManager/mTaskerInterface.h"



#define ENABLE_UART2_ISR_BUFFERS


// #define ENABLE_HARDWARE_UART_0
// #define ENABLE_HARDWARE_UART_1
// #define ENABLE_HARDWARE_UART_2

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

static const char *TAG = "uart_events";


 extern   uint8_t simple_uart2_receive_frame_for_calibration[4]; //adc2H, adc2L, adc5H, adc5L
 extern  bool flag_simple_uart2_receive_frame_for_calibration_updated ;


void UART1_ISR_Static(void* arg);
void UART2_ISR_Static_ByteRingBuffer(void* arg);
void UART2_ISR_Static_NoSplitRingBuffer(void* arg);
void UART2_ISR_Static_NoSplitRingBuffer_ForMeasurements(void *arg);
#ifdef USE_DEVFEATURE_MEASUREMENT_SYSTEM_CALIBRATION_METHOD_1
void IRAM_ATTR UART2_ISR_Static_NoSplitRingBuffer_ForMeasurements_Calibration(void *arg);
#endif // USE_DEVFEATURE_MEASUREMENT_SYSTEM_CALIBRATION_METHOD_1

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_UARTINFO_CTR) "uartinfo";

//https://www.reddit.com/r/esp32/comments/8k6ruk/uart_interrupt_not_working/


//  extern uint8_t uart_rss_tmp_buf_writer_index;
//  extern  char uart_rss_tmp_buffer_0[300];
//  extern  uint16_t uart_rss_tmp_buflen_0;
//  extern  char uart_rss_tmp_buffer_1[300];
//  extern  uint16_t uart_rss_tmp_buflen_1;
//  extern  char uart_rss_tmp_buffer_2[300];
//  extern  uint16_t uart_rss_tmp_buflen_2;
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

#include "3_Network/10_MQTT/mMQTT.h"

static uart_isr_handle_t *uart1_handle_console;
static uart_isr_handle_t *uart2_handle_console;

#define BLINK_GPIO GPIO_NUM_2



class mSerial :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mSerial(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);

    static const char* PM_MODULE_CORE__SERIAL__CTR;
    static const char* PM_MODULE_CORE__SERIAL__FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE__SERIAL__CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE__SERIAL__FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE__SERIAL__ID; }    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mSerial);    };
    #endif    

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    #if defined(ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS) && defined(USE_MODULE_CORE_FILESYSTEM)
    void Load_Module(bool erase = false);
    void Save_Module(void);
    bool Restore_Module(void);
    #endif // USE_MODULE_CORE_FILESYSTEM


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    
    
    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_UARTInfo(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();    
    void MQTTHandler_Sender();

    std::vector<struct handler<mSerial>*> mqtthandler_list;
    struct handler<mSerial> mqtthandler_settings_teleperiod;
    struct handler<mSerial> mqtthandler_uartinfo_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT

    /************************************************************************************************
     * SECITON: TOSORT
     ************************************************************************************************/

    char special_json_part_of_gps_buffer[300] = {0};
    uint16_t special_json_part_of_gps_buflen = 0;

    void Pre_Init_Pins();
    void StartISR_RingBuffers();

    // void IRAM_ATTR uart_intr_handle_u2(void *arg);

    uint8_t flag_init_buffers_and_start_isrs = false;

    uint16_t GetRingBufferDataAndClear(uint8_t uart_num, char* buffer, uint16_t buflen, char optional_read_until_char = 0, bool flag_clear_buffer_after_read = true);

    /***
     * UART1
     * 
     * */
    // #ifdef ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
    // #define RINGBUFFER_HANDLE_1_LENGTH 5000 //gps buffer
    // #else
    #define RINGBUFFER_HANDLE_1_LENGTH 2500
    // #endif
    void  init_UART1_RingBuffer();
    void  init_UART1_pins();
    void  init_UART1_ISR();
    // // Receive buffer to collect incoming data
    // uint8_t rxbuf1[RINGBUFFER_HANDLE_1_LENGTH] = {0};
    // // Register to collect data length
    // uint16_t urxlen1 = 0;


    //R288132N @ 1000    
    //                   5000 u2 of                                                  
    // R270100N 0054 TST UART1073479894 >> EMPTY    
    // enum RINGBUFFER_TYPE_IDS
    // {
    //   RINGBUFFER_TYPE_BYTEBUFFER_ID,
    //   RINGBUFFER_TYPE_NOSPLIT_BUFFER_ID,
    //   RINGBUFFER_TYPE_SPLIT_BUFFER_ID
    // }ringbuffer_t;

    int sendData(const char* logName, const char* data);

    /***
     * UART2
     * 
     * */
    #ifdef ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
    #define RINGBUFFER_HANDLE_2_LENGTH 200 //not used, lets not waste ram
    #else
    #define RINGBUFFER_HANDLE_2_LENGTH 5000
    #endif
    // #define RINGBUFFER_HANDLE_2_LENGTH 5000//15000
    void  init_UART2_RingBuffer();
    void  init_UART2_pins();
    void  init_UART2_ISR();
    // // Receive buffer to collect incoming data
    // char rxbuf2[RINGBUFFER_HANDLE_2_LENGTH] = {0};
    // // Register to collect data length
    // uint16_t urxlen2 = 0;

    // #define ENABLE_BUILTIN_LED_ACTIVITY_BLINKING
    
    struct UART_SETTINGS{
      bool receive_interrupts_enable = false;
      uint8_t configured = false;
      uint8_t initialised = false;
      uint32_t baud = 115200; //must be int
      RingbufHandle_t ringbuffer_handle;
      ringbuf_type_t ringbuffer_format_type = RINGBUF_TYPE_BYTEBUF;
      QueueHandle_t event_queue_handle;
      int ring_buffer_size_tx = (RINGBUFFER_HANDLE_2_LENGTH);// * 2); //1024*2
      int ring_buffer_size_rx = (RINGBUFFER_HANDLE_2_LENGTH);// * 2); //1024*2
      int event_queue_size = 10;
      struct GPIO{
        int8_t tx = UART_PIN_NO_CHANGE;
        int8_t rx = UART_PIN_NO_CHANGE;
      }gpio;
    };

    
    HardwareSerial* HWSerial  = nullptr;
    HardwareSerial* HWSerial1 = nullptr;
    HardwareSerial* HWSerial2 = nullptr;



    struct SETTINGS{
      #ifdef ENABLE_HARDWARE_UART_0
      UART_SETTINGS uart0;
      #endif
      #ifdef ENABLE_HARDWARE_UART_1
      UART_SETTINGS uart1;
      #endif
      #ifdef ENABLE_HARDWARE_UART_2
      UART_SETTINGS uart2;
      #endif
    }settings;

    void CommandSet_CreateFile_WithName(char* value);
    void CommandSet_SerialPrint_FileNames(const char* value);
    void CommandSet_WriteFile(const char* filename, const char* data = nullptr);
    void CommandSet_ReadFile(const char* filename);

    HardwareSerial* GetSerial(uint8_t index = 0);

    /**
     * lazy for now, hard coded 0xFF 0xFF
     * */
    // uint16_t GetRingBufferDataAndClearUntilSpecialDelimeter(uint8_t uart_num, char* buffer, uint16_t buflen);
    uint16_t GetSingleItemFromNoSplitRingBuffer(uint8_t uart_num, char* buffer, uint16_t buflen);   
  

    
    

};

#endif

#endif