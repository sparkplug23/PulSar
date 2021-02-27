#ifndef _USE_MODULE_DRIVERS_SERIAL_UART_H
#define _USE_MODULE_DRIVERS_SERIAL_UART_H 0.3

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_SERIAL_UART

#define ENABLE_UART2_ISR_BUFFERS


#include <string.h>
#include <strings.h>


#include "freertos/ringbuf.h"
static char tx_item[] = "test_item";
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

void uart_intr_handle_u2_static(void* arg);


//https://www.reddit.com/r/esp32/comments/8k6ruk/uart_interrupt_not_working/


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

#define BLINK_GPIO GPIO_NUM_2

class mSerialUART{

  private:
  public:
    mSerialUART(){};
    int8_t Tasker(uint8_t function);
    // int8_t Tasker(uint8_t function, JsonObjectConst obj);   

    uart_isr_handle_t *handle_console_uart2;
    int8_t Tasker_Web(uint8_t function);

    // void IRAM_ATTR uart_intr_handle_u2(void *arg);

    QueueHandle_t uart_queue;
    const int uart_buffer_size = (1024 * 2);

    void  init_UART2_RingBuffer();
    void  init_UART2_ISR();

    static void IRAM_ATTR uart_intr_handle_u2(void);

    // Receive buffer to collect incoming data
    uint8_t rxbuf2[256];
    // Register to collect data length
    uint16_t urxlen2;

    #define RINGBUFFER_HANDLE_2_LENGTH 100000
    struct UART2_SETTINGS{
      RingbufHandle_t ringbuffer_handle;
      uint8_t initialised = false;
    }uart2_settings;

    void init();
    void pre_init();
    int8_t pin = -1;
    struct SETTINGS{
      uint8_t fEnableModule = false;
      uint8_t fShowManualSlider = false;
    }settings;

    void CommandSet_CreateFile_WithName(char* value);
    void CommandSet_SerialPrint_FileNames(const char* value);
    void CommandSet_WriteFile(const char* filename, const char* data = nullptr);
    void CommandSet_ReadFile(const char* filename);

    int8_t CheckAndExecute_JSONCommands();
    void parse_JSONCommand(void);

    uint8_t ConstructJSON_Scene(uint8_t json_method = 0);
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mSerialUART>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mSerialUART> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mSerialUART> mqtthandler_sensor_ifchanged;
    struct handler<mSerialUART> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif