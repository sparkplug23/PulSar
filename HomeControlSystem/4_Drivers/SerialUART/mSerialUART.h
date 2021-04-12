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
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
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


/*

#ifdef USE_DEVFEATURE_UART_TEST

const uart_port_t uart_num = UART_NUM_2;

// UART Handler
// static void IRAM_ATTR uart_intr_handle(void *arg) {
//   // uint16_t rx_fifo_len;
  
//   // //status = UART0.int_st.val;            // Read UART interrupt Status
//   // rx_fifo_len = UART0.status.rxfifo_cnt;  // Read number of bytes in UART buffer
  
//   // while(rx_fifo_len){
//   //  rxbuf[nrBytes++] = READ_PERI_REG(UART_FIFO_AHB_REG(0));//UART0.fifo.rw_byte;       // Read all bytes
//   //  rx_fifo_len--;
//   // }

//   // UART_RECEIVED = 1;                      // Set flag

//   // // After reading bytes from buffer clear UART interrupt status
//   // uart_clear_intr_status(uart_num , UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
// }

static void IRAM_ATTR uart_intr_handle4(void* arg)
{
  //uart_write_bytes(UART_NUM_0, (const char*) "uart_intr_handle4\n\r", 20);
}


void UART_Setup(){

gpio_pad_select_gpio(BLINK_GPIO);
    
    * Set the GPIO as a push/pull output *
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    init_UART0();
    init_UART1();
    // pCONT_serial- init_UART2();


//  uart_config_t uart_config = {
// 	      .baud_rate = 115200,
// 	      .data_bits = UART_DATA_8_BITS,
// 	      .parity = UART_PARITY_ODD,
// 	      .stop_bits = UART_STOP_BITS_1,
// 	      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
// 	    };
// 	    ESP_ERROR_CHECK(uart_param_config(uart_num , &uart_config));
// 	    ESP_ERROR_CHECK(uart_set_pin(uart_num , UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));   // Set UART pins (using UART0 default pins ie no changes.)
// 	    ESP_ERROR_CHECK(uart_driver_install(uart_num , BUF_SIZE * 2, BUF_SIZE * 2, 10, NULL, 0));                    // Install UART driver, and get the queue.        
// 	    // ESP_ERROR_CHECK(uart_driver_install(uart_num , 0, 0, 0, NULL, 0));                    // Install UART driver, and get the queue.        
// 	    ESP_ERROR_CHECK(uart_isr_free(uart_num ));                                                                  // Release the pre registered UART handler/subroutine
// 	    ESP_ERROR_CHECK(uart_isr_register(uart_num ,uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, NULL));             // Register new UART subroutine
// 	    ESP_ERROR_CHECK(uart_enable_rx_intr(uart_num ));


// int ret;
// 	esp_log_level_set(TAG, ESP_LOG_INFO);

// 	* Configure parameters of an UART driver,
// 	* communication pins and install the driver *
// 	uart_config_t uart_config = {
// 		.baud_rate = 115200,
// 		.data_bits = UART_DATA_8_BITS,
// 		.parity = UART_PARITY_DISABLE,
// 		.stop_bits = UART_STOP_BITS_1,
// 		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
// 	};

//   Serial2.begin(115200);




	// ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));

	// //Set UART log level
	// esp_log_level_set(TAG, ESP_LOG_INFO);

	// //Set UART pins (using UART0 default pins ie no changes.)
	// ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

	// //Install UART driver, and get the queue.
	// ESP_ERROR_CHECK(
  //   uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0)
  //    );

  //  uart_write_bytes(
  //  UART_NUM_2,
  //  (const char*) "Setup done, port uart2 working", 31);

	// release the pre registered UART handler/subroutine
	// ESP_ERROR_CHECK(uart_isr_free(EX_UART_NUM));

	// // register new UART subroutine
	// ESP_ERROR_CHECK(uart_isr_register(EX_UART_NUM,uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console));

	// // enable RX interrupt
	// ESP_ERROR_CHECK(uart_enable_rx_intr(EX_UART_NUM));

// 	#if (NOTASK == 1)
// 	while(1)
// 	{
// 		vTaskDelay(1000);
// 	}
// 	#else
		// xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
	// #endif
}

void init_UART0(){

	esp_log_level_set(TAG, ESP_LOG_INFO);

	* Configure parameters of an UART driver,
	* communication pins and install the driver *
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};

  //PORT 0 for debug
  ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));

	//Set UART log level
	esp_log_level_set(TAG, ESP_LOG_INFO);

	//Set UART pins (using UART0 default pins ie no changes.)
	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

	//Install UART driver, and get the queue.
	ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0));

	// release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(uart_isr_free(UART_NUM_0));

	// register new UART subroutine
	ESP_ERROR_CHECK(uart_isr_register(UART_NUM_0,uart_intr_handle_u0, NULL, ESP_INTR_FLAG_IRAM, &handle_console_u0));

	// enable RX interrupt
	ESP_ERROR_CHECK(uart_enable_rx_intr(UART_NUM_0));

  uart_write_bytes(
   UART_NUM_0,
   (const char*) "UART0 - Reboot\n\r", 17);
  
}

void init_UART1(){

	// esp_log_level_set(TAG, ESP_LOG_INFO);

	* Configure parameters of an UART driver,
	* communication pins and install the driver *
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};

  // Serial2.begin(115200);
  // Serial2.println("115200this");

  //PORT 0 for debug
  ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));

	//Set UART log level
	// esp_log_level_set(TAG, ESP_LOG_INFO);

	//Set UART pins (using UART0 default pins ie no changes.)
	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, 25, 26, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

	//Install UART driver, and get the queue.
	ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0));

	// release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(uart_isr_free(UART_NUM_1));

	// register new UART subroutine
	ESP_ERROR_CHECK(uart_isr_register(UART_NUM_1,uart_intr_handle_u1, NULL, ESP_INTR_FLAG_IRAM, &handle_console_u0));

	// enable RX interrupt
	ESP_ERROR_CHECK(uart_enable_rx_intr(UART_NUM_1));

  uart_write_bytes(
   UART_NUM_1,
   (const char*) "UART1 - Reboot\n\r", 17);

  
}

// #define UART2_METHOD_NO_QUEUE
// #define UART2_METHOD_QUEUE

void init_UART2(){

#ifdef UART2_METHOD_NO_QUEUE
	// esp_log_level_set(TAG, ESP_LOG_INFO);

	* Configure parameters of an UART driver,
	* communication pins and install the driver *
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};

  // Serial2.begin(115200);
  // Serial2.println("115200this");

  //PORT 0 for debug
  ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));

	//Set UART log level
	// esp_log_level_set(TAG, ESP_LOG_INFO);

	//Set UART pins (using UART0 default pins ie no changes.)
	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

	//Install UART driver, and get the queue.
	ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0));

	// release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(uart_isr_free(UART_NUM_2));

	// register new UART subroutine
	ESP_ERROR_CHECK(uart_isr_register(UART_NUM_2,uart_intr_handle_u2, NULL, ESP_INTR_FLAG_IRAM, &handle_console_u0));

	// enable RX interrupt
	ESP_ERROR_CHECK(uart_enable_rx_intr(UART_NUM_2));
#endif // UART2_METHOD_NO_QUEUE
  
#ifdef UART2_METHOD_QUEUE

 int ret;
    esp_log_level_set(TAG, ESP_LOG_INFO);

    * Configure parameters of an UART driver,
     * communication pins and install the driver *
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));


// // Setup UART buffered IO with event queue
// const int uart_buffer_size = (1024 * 2);
// QueueHandle_t uart_queue;

    //Set UART log level
    // esp_log_level_set(TAG, ESP_LOG_INFO);
    // ESP_LOGI(TAG, "uart event main");
    //Set UART pins (using UART0 default pins ie no changes.)
    // ESP_ERROR_CHECK(uart_set_pin(EX_UART_NUM, GPIO_NUM_4, GPIO_NUM_5, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	// ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    //Install UART driver, and get the queue.
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));

	// // release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(uart_isr_free(UART_NUM_2));
  //   // ESP_LOGI(TAG, "uart init interrupt");
  //   // ESP_ERROR_CHECK(uart_enable_intr_mask(EX_UART_NUM, UART_INTR_MASK));
  // //    //Set uart pattern detect function.
  // //    uart_enable_pattern_det_intr(EX_UART_NUM, '+', PATTERN_CHR_NUM, 10000, 10, 10);
  // //    //Reset the pattern queue length to record at most 20 pattern positions.
  // //    uart_pattern_queue_reset(EX_UART_NUM, 20);
  //   // ESP_LOGI(TAG, "uart reg isr interrupt");
    ESP_ERROR_CHECK(uart_isr_register(UART_NUM_2,
    uart_intr_handle_u2,
    // uart_intr_handle4,
    
     NULL, 
    
    ESP_INTR_FLAG_IRAM,// | ESP_INTR_FLAG_LOWMED,
    // ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LOWMED,
    // ESP_INTR_FLAG_LOWMED, 
    
    handle_console4));

  //   // ESP_LOGI(TAG, "uart rx isr enable");
  //   //ESP_LOGI(TAG, "Return from UART ISR %X", ret);
    // ESP_ERROR_CHECK(uart_enable_rx_intr(UART_NUM_2));
    //Create a task to handler UART event from ISR

    //xTaskCreatePinnedToCore
    // xTaskCreatePinnedToCore()
    
    // 	uart_queue = xQueueCreate( 10, sizeof( struct AMessage * ) );
 	  // if( uart_queue == 0 )
  	// {
    //   // Serial.println("uart_queue FAILED!"); delay(4000);
  	// 	// Failed to create the queue.
  	// }
    //   // Serial.println("uart_queue SUCESS!"); delay(4000);

    // 	uart0_queue = xQueueCreate( 10, sizeof( struct AMessage * ) );
 	  // if( uart0_queue == 0 )
  	// {
    //   // Serial.println("uart_queue FAILED!"); delay(4000);
  	// 	// Failed to create the queue.
  	// }
      // Serial.println("uar/t_queue SUCESS!"); delay(4000);

    
    // xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
#endif // UART2_METHOD_QUEUE




  // uart_write_bytes(
  //  UART_NUM_2,
  //  (const char*) "UART2 - Reboot\n\r", 17);

  
}


void UART_Loop(){

  // Serial.println("loop");

  // if(urxlen){
  //   for(int ii=0;ii<urxlen;ii++){
  //     Serial.print((char)rxbuf[ii]);
  //   }
  //   Serial.println();
  //   urxlen = 0;
  // }


}


#endif // UART_TEST

*/


  // yield();
// #ifdef USE_DEVFEATURE_UART_TEST
//   // UART_Loop();

//   // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos_additions.html

// /**
//  * @brief   Get information about ring buffer status
//  *
//  * Get information of the a ring buffer's current status such as
//  * free/read/write pointer positions, and number of items waiting to be retrieved.
//  * Arguments can be set to NULL if they are not required.
//  *
//  * @param[in]   xRingbuffer     Ring buffer to remove from the queue set
//  * @param[out]  uxFree          Pointer use to store free pointer position
//  * @param[out]  uxRead          Pointer use to store read pointer position
//  * @param[out]  uxWrite         Pointer use to store write pointer position
//  * @param[out]  uxItemsWaiting  Pointer use to store number of items (bytes for byte buffer) waiting to be retrieved
//  */
// // void vRingbufferGetInfo(RingbufHandle_t xRingbuffer, UBaseType_t *uxFree, UBaseType_t *uxRead, UBaseType_t *uxWrite, UBaseType_t *uxItemsWaiting);
// // RingbufHandle_t xRingbuffer;
// // UBaseType_t *uxFree;
// // UBaseType_t *uxRead;
// // UBaseType_t *uxWrite;
// // UBaseType_t *uxItemsWaiting;
// // vRingbufferGetInfo(xRingbuffer, uxFree, uxRead, uxWrite, uxItemsWaiting);

// #ifdef ENABLE_DEVFEATURE_BYTEBUFFER
//   if(abs(millis() - tick_fill)>1000){ tick_fill = millis();
//     Serial.println("Writing into buffer");

//     char message[100] = {0};
//     for(int i=0;i<9;i++){
//       sprintf(message+strlen(message),"%d",number);
//     }
//     number++;

//     UBaseType_t res =  xRingbufferSend(buf_handle, message, strlen(message), pdMS_TO_TICKS(1000));
//     if (res != pdTRUE) {
//       Serial.printf("Failed to send item\n\r");
//     }else{
//       // Serial.printf("messaged added\n\r");
//     }
    
//   //   // freeSize = xRingbufferGetCurFreeSize(buf_handle);
//   //   // Serial.printf("freeSize1=%d\n\r",freeSize);

//   //   // UBaseType_t res2 =  xRingbufferSend(buf_handle, message, sizeof(message), pdMS_TO_TICKS(1000));
//   //   // if (res2 != pdTRUE) {
//   //   //   Serial.printf("Failed to send item\n\r");
//   //   // }else{
//   //   //   // Serial.printf("messaged added\n\r");
//   //   // }

//   //   // freeSize = xRingbufferGetCurFreeSize(buf_handle);
//   //   // Serial.printf("freeSize2=%d\n\r",freeSize);


//   //   //  for(int i=0;i<10;i++){
//   //   //   sprintf(message+strlen(message),"a",number);
//   //   // }
//   //   // res =  xRingbufferSend(buf_handle, message, 
//   //   // //sizeof(tx_item), 
//   //   // sizeof(message),
//   //   // pdMS_TO_TICKS(1000));
//   //   // if (res != pdTRUE) {
//   //   //   Serial.printf("Failed to send item\n\r");
//   //   // }else{
//   //   //   // Serial.printf("messaged added\n\r");
//   //   // }
    
//   //   // Serial.printf("buff>>%s",bu);

//   }


// #endif // test

//   if(abs(millis() - tick_read)>=10000){ tick_read = millis();
//     //Receive an item from allow-split ring buffer

//     // xRingbufferPrintInfo(buf_handle);
//     // //Receive an item from no-split ring buffer
//     // size_t item_size;
//     // char *item = (char *)xRingbufferReceive(buf_handle, &item_size, pdMS_TO_TICKS(1000));

//     // //Check received item
//     // if (item != NULL) {
//     //     //Print item
//     //     for (int i = 0; i < item_size; i++) {
//     //         printf("%c", item[i]);
//     //     }
//     //     printf("\n");
//     //     //Return Item
//     //     vRingbufferReturnItem(buf_handle, (void *)item);
//     // } else {
//     //     //Failed to receive item
//     //     printf("Failed to receive item\n");
//     // }


//     // while(
//     //   (freeSize = xRingbufferGetCurFreeSize(buf_handle))
//     //   <
//     //   984
//     //   ){
    
//     //   Serial.printf("freeSize while = %d\n\r",freeSize);

//     //   if(abs(millis() - tick_read)>10000){
//     //     break;
//     //   }
      
//     //   size_t item_size1, item_size2;
//     //   char *item1, *item2;
//     //   BaseType_t ret = xRingbufferReceiveSplit(buf_handle, (void **)&item1, (void **)&item2, &item_size1, &item_size2, pdMS_TO_TICKS(1000));

//     //   //Check received item
//     //   if (ret == pdTRUE && item1 != NULL) {
//     //     Serial.printf("item1 size = %d\n\r", item_size1);
//     //     for (int i = 0; i < item_size1; i++) {
//     //       Serial.printf("%c", item1[i]);
//     //     }
//     //     vRingbufferReturnItem(buf_handle, (void *)item1);
//     //     //Check if item was split
//     //     if (item2 != NULL) {
//     //       for (int i = 0; i < item_size2; i++) {
//     //         Serial.printf("%c", item2[i]);
//     //       }
//     //       vRingbufferReturnItem(buf_handle, (void *)item2);
//     //     }
//     //     Serial.printf("\n");
//     //   } else {
//     //     //Failed to receive item
//     //     Serial.printf("Failed to receive item\n");
//     //   }

//     // xRingbufferPrintInfo(uart2_settings.ringbuffer_handle);
//     // //Receive an item from no-split ring buffer
//     size_t item_size;
//     char*  item = (char *)xRingbufferReceive(pCONT_serial->uart2_settings.ringbuffer_handle, &item_size, pdMS_TO_TICKS(1000));

//     //Check received item
//     if (item != NULL) {
//       Serial.printf("UART2:>%d %d sizeB=%d\n\r",item_size,millis(),xRingbufferGetCurFreeSize(pCONT_serial->uart2_settings.ringbuffer_handle));
//       //Print item
//       for (int i = 0; i < item_size; i++) {
//           Serial.printf("%c", item[i]);
//       }
//       //Return Item
//       vRingbufferReturnItem(pCONT_serial->uart2_settings.ringbuffer_handle, (void *)item);
//       Serial.printf("\n\r sizeA=%d\n\r",xRingbufferGetCurFreeSize(pCONT_serial->uart2_settings.ringbuffer_handle));
//     } else {
//       //Failed to receive item
//       Serial.printf("Failed to receive item\n");
//     }




// // xRingbufferPrintInfo(buf_handle);
  
//     }
//     // read half out

//     //print whole buffer
//     // for(int ii=0;ii<100;ii++){
//     //   Serial.print((char)buffer_test[ii]);
//     // }
//     // Serial.println();

//   // }