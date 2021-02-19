/*
   Copyright (c) 2020, my_name_here
   All rights reserved.
   https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf#uart-reg-summ
*/

// From my computer

#define D_USER_MICHAEL // maybe undef later?
#define USE_DEVFEATURE_UART_TEST

#define USE_DEVFEATURE_DISABLE_ALL_PROJECT_FOR_TESTING

#define NO_GLOBAL_MDNS
#include "1_TaskerManager/mTaskerManager.h"

// #define USE_DECLARE_AT_COMPILE_TO_DEBU
//G

#define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

#ifdef ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING
void hw_wdt_disable(){
  *((volatile uint32_t*) 0x60000900) &= ~(1); // Hardware WDT OFF 
}

void hw_wdt_enable(){ 
  *((volatile uint32_t*) 0x60000900) |= 1; // Hardware WDT ON 
}
#endif

#ifdef USE_DEVFEATURE_UART_TEST
/* 
UART Interrupt Example
*/
#ifdef CORE_DEBUG_LEVEL
#undef CORE_DEBUG_LEVEL
#endif

#define CORE_DEBUG_LEVEL 3
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

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


#define BUF_HANDLE_LENGTH 10000

#define BLINK_GPIO GPIO_NUM_2

static const char *TAG = "uart_events";

// Setup UART buffered IO with event queue
const int uart_buffer_size = (1024 * 2);
QueueHandle_t uart_queue;


static QueueHandle_t buffer_queue;
char buffer_test[100];

    //Create ring buffer
    RingbufHandle_t buf_handle;

/**
 * This example shows how to use the UART driver to handle UART interrupt.
 *
 * - Port: UART0
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: on
 * - Pin assignment: TxD (default), RxD (default)
 */

  void   init_UART0();
  void   init_UART1();
  void   init_UART2();

#define EX_UART_NUM UART_NUM_0
#define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)
static QueueHandle_t uart0_queue;
static QueueHandle_t queue_rx_u2;

// Both definition are same and valid
//static uart_isr_handle_t *handle_console;
static intr_handle_t handle_console;
static intr_handle_t handle_console_u0;

// Receive buffer to collect incoming data
uint8_t rxbuf[256];
// Register to collect data length
uint16_t urxlen;

#define NOTASK 0

//https://www.reddit.com/r/esp32/comments/8k6ruk/uart_interrupt_not_working/
static uart_isr_handle_t *handle_console4;

uint8_t argISR;

static const char *TAG4 = "uart_events";

/**
 * This example shows how to use the UART driver to handle special UART events.
 *
 * It also reads data from UART0 directly, and echoes it to console.
 *
 * - Port: UART0
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: on
 * - Pin assignment: TxD (default), RxD (default)
 */
  struct AMessage
   {
  	char ucMessageID;
  	char ucData[ 20 ];
   } xMessage;
 
   QueueHandle_t xQueue;

// This will run forever! I dont want this, as a thread, use a flag in the main loop instead
static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);

     
    // Task to create a queue and post a value.
  //  void vATask( void *pvParameters )
  //  {
//    struct AMessage *pxMessage;
 
//   	// Create a queue capable of containing 10 pointers to AMessage structures.
//   	// These should be passed by pointer as they contain a lot of data.
//   	xQueue = xQueueCreate( 10, sizeof( struct AMessage * ) );
//  	  if( xQueue == 0 )
//   	{
//   		// Failed to create the queue.
//   	}

// //  uart0_queue


//   	// ...
 
//   	// Send a pointer to a struct AMessage object.  Don't block if the
//   	// queue is already full.
//   	pxMessage = & xMessage;
//   	xQueueSend( xQueue, ( void * ) &pxMessage, ( TickType_t ) 0 );
 
//   	// ... Rest of task code.
//   //  }
 
//   //  // Task to receive from the queue.
//   //  void vADifferentTask( void *pvParameters )
//   //  {
//    struct AMessage *pxRxedMessage;
 
//   // 	if( xQueue != 0 )
//   // 	{
//   // 		// Receive a message on the created queue.  Block for 10 ticks if a
//   // 		// message is not immediately available.
//   		if( xQueueReceive( xQueue, &( pxRxedMessage ), ( TickType_t ) 10 ) )
//   		{
//   			// pcRxedMessage now points to the struct AMessage variable posted
//   			// by vATask.
//   		}
  // 	}

  uint32_t time1 = millis();
  
  
      Serial.println("start");
  
    // for(;;) {

      // yield();
      // ESP_LOGI(TAG, "uart[%d] event:", UART_NUM_2);

 
  	// Create a queue capable of containing 10 pointers to AMessage structures.
  	// These should be passed by pointer as they contain a lot of data.
  	// uart0_queue = xQueueCreate( 10, sizeof( struct AMessage * ) );
  	// if( uart0_queue == 0 )
  	// {
  	// 	// Failed to create the queue.
  	// }
  
  if(abs(millis()-time1)>1000){
    time1 = millis();
      Serial.println("for");
  }
  

/*

        //Waiting for UART event.
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
          
      Serial.println("art[%d] even1");

            bzero(dtmp, RD_BUF_SIZE);
            ESP_LOGI(TAG, "uart[%d] event:", UART_NUM_2);
            switch(event.type) {
                //Event of UART receving data
                // *We'd better handler data event fast, there would be much more data events than
                // other types of events. If we take too much time on data event, the queue might
                // be full.*
                case UART_DATA:
                    ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
                    uart_read_bytes(UART_NUM_2, dtmp, event.size, portMAX_DELAY);
                    ESP_LOGI(TAG, "[DATA EVT]:");
                    uart_write_bytes(UART_NUM_2, (const char*) dtmp, event.size);
                    break;
                //Event of HW FIFO overflow detected
                // case UART_FIFO_OVF:
                //     ESP_LOGI(TAG, "hw fifo overflow");
                //     // If fifo overflow happened, you should consider adding flow control for your application.
                //     // The ISR has already reset the rx FIFO,
                //     // As an example, we directly flush the rx buffer here in order to read more data.
                //     uart_flush_input(UART_NUM_2);
                //     xQueueReset(uart0_queue);
                //     break;
                // //Event of UART ring buffer full
                // case UART_BUFFER_FULL:
                //     ESP_LOGI(TAG, "ring buffer full");
                //     // If buffer full happened, you should consider encreasing your buffer size
                //     // As an example, we directly flush the rx buffer here in order to read more data.
                //     uart_flush_input(UART_NUM_2);
                //     xQueueReset(uart0_queue);
                //     break;
                // //Event of UART RX break detected
                // case UART_BREAK:
                //     ESP_LOGI(TAG, "uart rx break");
                //     break;
                // //Event of UART parity check error
                // case UART_PARITY_ERR:
                //     ESP_LOGI(TAG, "uart parity error");
                //     break;
                // //Event of UART frame error
                // case UART_FRAME_ERR:
                //     ESP_LOGI(TAG, "uart frame error");
                //     break;
                // //UART_PATTERN_DET
                // case UART_PATTERN_DET:{
                //     uart_get_buffered_data_len(UART_NUM_2, &buffered_size);
                //     int pos = uart_pattern_pop_pos(UART_NUM_2);
                //     ESP_LOGI(TAG, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
                //     if (pos == -1) {
                //         // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                //         // record the position. We should set a larger queue size.
                //         // As an example, we directly flush the rx buffer here.
                //         uart_flush_input(UART_NUM_2);
                //     } else {
                //         uart_read_bytes(UART_NUM_2, dtmp, pos, 100 / portTICK_PERIOD_MS);
                //         uint8_t pat[PATTERN_CHR_NUM + 1];
                //         memset(pat, 0, sizeof(pat));
                //         uart_read_bytes(UART_NUM_2, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
                //         ESP_LOGI(TAG, "read data: %s", dtmp);
                //         ESP_LOGI(TAG, "read pat : %s", pat);
                //     }
                // }
                //     break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
                    break;
            }
        } //queue
        // yield(); //feed WDT?
        
    */


    // } //end for 



    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}




void blink_task(void *pvParameter)
{
    gpio_pad_select_gpio(BLINK_GPIO);
    
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    
    while(1) {
        /* Blink off (output low) */
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
  //        uart_write_bytes(
  // //  EX_UART_NUM, 
  //  UART_NUM_0,
  //  (const char*) "blink_task", 7);

    }
}
// /*
//  * Define UART interrupt subroutine to ackowledge interrupt
//  */
// static void IRAM_ATTR uart_intr_handle(void *arg)
// {
//   uint16_t rx_fifo_len, status;
//   uint16_t i;
  
//     gpio_pad_select_gpio(BLINK_GPIO);
    
//     /* Set the GPIO as a push/pull output */
//     gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


//   status = UART2.int_st.val; // read UART interrupt Status
//   rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer
  
//   while(rx_fifo_len){
//    rxbuf[i++] = UART2.fifo.rw_byte; // read all bytes
//    rx_fifo_len--;
//         gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//  }
    
//         // /* Blink off (output low) */
//         // gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//         /* Blink on (output high) */
//         // gpio_set_level(BLINK_GPIO, 1);
  
// //  // after reading bytes from buffer clear UART interrupt status
//  uart_clear_intr_status(EX_UART_NUM, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// // // a test code or debug code to indicate UART receives successfully,
// // // you can redirect received byte as echo also
//  uart_write_bytes(
//   //  EX_UART_NUM, 
//    UART_NUM_0,
//    (const char*) "RX Done", 7);

// }

/*
 * Define UART interrupt subroutine to ackowledge interrupt
 */
static void IRAM_ATTR uart_intr_handle_u0(void *arg)
{
  uint16_t rx_fifo_len, status;
  uint16_t i;
  
    gpio_pad_select_gpio(BLINK_GPIO);
    
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


  status = UART0.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART0.status.rxfifo_cnt; // read number of bytes in UART buffer
  char single_byte = '-';
  
  while(rx_fifo_len){
    single_byte = UART0.fifo.rw_byte;
    rxbuf[i++] = single_byte; // read all bytes
    rx_fifo_len--;
    gpio_set_level(BLINK_GPIO, rx_fifo_len%2);
  }
    
        // /* Blink off (output low) */
        // gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
        /* Blink on (output high) */
        // gpio_set_level(BLINK_GPIO, 1);
  
//  // after reading bytes from buffer clear UART interrupt status
 uart_clear_intr_status(UART_NUM_0, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// // a test code or debug code to indicate UART receives successfully,
// // you can redirect received byte as echo also
//  uart_write_bytes(
//   //  EX_UART_NUM, 
//    UART_NUM_2,
//    (const char*) "RX Done", 7);

// uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);


   uart_write_bytes(
   UART_NUM_0,
   (const char*) "Setup 0isr, port uart0 workin\n\r", 32);
   uart_write_bytes(
   UART_NUM_2,
   (const char*) "Setup 0isr, port uart2 workin\n\r", 32);
//  uart_write_bytes(
//    UART_NUM_0,
//    (const char*) single_byte, 1);

}

/*
 * Define UART interrupt subroutine to ackowledge interrupt
 */
static void IRAM_ATTR uart_intr_handle_u1(void *arg)
{
  uint16_t rx_fifo_len, status;
  uint16_t i;
  
    gpio_pad_select_gpio(BLINK_GPIO);
    
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


  status = UART1.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART1.status.rxfifo_cnt; // read number of bytes in UART buffer
  char single_byte = '-';
  
  while(rx_fifo_len){
    single_byte = UART1.fifo.rw_byte;
    rxbuf[i++] = single_byte; // read all bytes
    rx_fifo_len--;
    gpio_set_level(BLINK_GPIO, rx_fifo_len%2);
  }
    
        // /* Blink off (output low) */
        // gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
        /* Blink on (output high) */
        // gpio_set_level(BLINK_GPIO, 1);
  
//  // after reading bytes from buffer clear UART interrupt status
 uart_clear_intr_status(UART_NUM_1, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// // a test code or debug code to indicate UART receives successfully,
// // you can redirect received byte as echo also
//  uart_write_bytes(
//   //  EX_UART_NUM, 
//    UART_NUM_2,
//    (const char*) "RX Done", 7);

// uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);


   uart_write_bytes(
   UART_NUM_0,
   (const char*) "Setup 1isr, port uart0 workin\n\r", 32);
   uart_write_bytes(
   UART_NUM_2,
   (const char*) "Setup 1isr, port uart2 workin\n\r", 32);
//  uart_write_bytes(
//    UART_NUM_0,
//    (const char*) single_byte, 1);

}

/*
 * Define UART interrupt subroutine to ackowledge interrupt
 */
static void IRAM_ATTR uart_intr_handle_u2(void *arg)
{
  uint16_t rx_fifo_len, status;
  uint16_t i = 0;
  
  // uart_write_bytes(
  //  UART_NUM_1,
  //  (const char*) "UART2 - uart_intr_handle_u2\n\r", 30);



    gpio_pad_select_gpio(BLINK_GPIO);
    
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


  status = UART2.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer
  char single_byte = '-';
  
  while(rx_fifo_len){
    single_byte = UART2.fifo.rw_byte;
    rxbuf[i++] = single_byte; // read all bytes
    rx_fifo_len--;
    gpio_set_level(BLINK_GPIO, rx_fifo_len%2);
    // uart_tx_chars(UART_NUM_1,  (const char*)single_byte, 1);
    // uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);
  }
urxlen = i;
  // for(int ii=0;ii<i;ii++)
    // if(i<30){
      // uart_write_bytes(   UART_NUM_1,   (const char*) rxbuf, i-1);
    // } 
        // /* Blink off (output low) */
        // gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
        /* Blink on (output high) */
        // gpio_set_level(BLINK_GPIO, 1);
  
//  // after reading bytes from buffer clear UART interrupt status
 uart_clear_intr_status(UART_NUM_2, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// // a test code or debug code to indicate UART receives successfully,
// // you can redirect received byte as echo also
//  uart_write_bytes(
//   //  EX_UART_NUM, 
//    UART_NUM_2,
//    (const char*) "RX Done", 7);

// uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);


  //  uart_write_bytes(
  //  UART_NUM_0,
  //  (const char*) "Setup 2isr, port uart0 workin\n\r", 32);
  //  uart_write_bytes(
  //  UART_NUM_2,
  //  (const char*) "Setup 2isr, port uart2 workin\n\r", 32);

//  uart_write_bytes(
//    UART_NUM_0,
//    (const char*) single_byte, 1);

  // uart_write_bytes(
  //  UART_NUM_1,
  //  (const char*) "UART2 - isr END\n\r", 18);
}


/*
 * Define UART interrupt subroutine to ackowledge interrupt
 */
static void IRAM_ATTR uart_intr_handle_u2_q(void *arg)
{
  uint16_t rx_fifo_len, status;
  uint16_t i = 0;
  
  uart_write_bytes(
   UART_NUM_1,
   (const char*) "UART2 - uart_intr_handle_u2_q\n\r", 32);

// Read data from UART.
// const int uart_num = UART_NUM_2;
// uint8_t data[128];
// int length = 0;
// ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));
// length = uart_read_bytes(uart_num, data, length, 100);

//If the data in the Rx FIFO buffer is no longer needed, you can clear the buffer by calling uart_flush().


    // gpio_pad_select_gpio(BLINK_GPIO);
    
    // /* Set the GPIO as a push/pull output */
    // gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


//   status = UART2.int_st.val; // read UART interrupt Status
//   rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer
//   char single_byte = '-';
  
//   while(rx_fifo_len){
//     single_byte = UART2.fifo.rw_byte;
//     rxbuf[i++] = single_byte; // read all bytes
//     rx_fifo_len--;
//     gpio_set_level(BLINK_GPIO, rx_fifo_len%2);
//     // uart_tx_chars(UART_NUM_1,  (const char*)single_byte, 1);
//     // uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);
//   }
// urxlen = i;
  // for(int ii=0;ii<i;ii++)
    // if(i<30){
      // uart_write_bytes(   UART_NUM_1,   (const char*) rxbuf, i-1);
    // } 
        // /* Blink off (output low) */
        // gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
        /* Blink on (output high) */
        // gpio_set_level(BLINK_GPIO, 1);
  
//  // after reading bytes from buffer clear UART interrupt status
 uart_clear_intr_status(UART_NUM_2, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// // a test code or debug code to indicate UART receives successfully,
// // you can redirect received byte as echo also
//  uart_write_bytes(
//   //  EX_UART_NUM, 
//    UART_NUM_2,
//    (const char*) "RX Done", 7);

// uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);


  //  uart_write_bytes(
  //  UART_NUM_0,
  //  (const char*) "Setup 2isr, port uart0 workin\n\r", 32);
  //  uart_write_bytes(
  //  UART_NUM_2,
  //  (const char*) "Setup 2isr, port uart2 workin\n\r", 32);

//  uart_write_bytes(
//    UART_NUM_0,
//    (const char*) single_byte, 1);

  uart_write_bytes(
   UART_NUM_1,
   (const char*) "UART2 - isr ENDq\n\r", 19);
}



void UART_Setup();
void UART_Loop();
#endif // USE_DEVFEATURE_UART_TEST

#ifdef USE_DECLARE_AT_COMPILE_TO_DEBUG
//include file version

#endif // USE_DECLARE_AT_COMPILE_TO_DEBUG

/*



/*********************************************************************************************
 * Hardware related
\*********************************************************************************************

#ifdef ESP8266

void HwWdtDisable(void) {
  *((volatile uint32_t*) 0x60000900) &= ~(1);  // Hardware WDT OFF
}

void HwWdtEnable(void) {
  *((volatile uint32_t*) 0x60000900) |= 1;     // Hardware WDT ON
}

void WdtDisable(void) {
  ESP.wdtDisable();
  HwWdtDisable();
}

void WdtEnable(void) {
  HwWdtEnable();
  ESP.wdtEnable(0);
}

#endif  // ESP8266*/

/*
without energyusage in rtc
RAM:   [======    ]  64.7% (used 53000 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510748 bytes from 1023984 bytes)
// with energy rtc
RAM:   [======    ]  64.8% (used 53112 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510828 bytes from 1023984 bytes)
//
RAM:   [======    ]  64.2% (used 52576 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510764 bytes from 1023984 bytes)
RAM:   [======    ]  63.1% (used 51680 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510224 bytes from 1023984 bytes)

RAM:   [======    ]  63.1% (used 51656 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510224 bytes from 1023984 bytes)

RAM:   [======    ]  63.0% (used 51640 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510224 bytes from 1023984 bytes)
//
RAM:   [======    ]  59.6% (used 48840 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510176 bytes from 1023984 bytes)


RAM:   [======    ]  63.5% (used 52040 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510352 bytes from 1023984 bytes)

RAM:   [======    ]  63.5% (used 52040 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510268 bytes from 1023984 bytes)

RAM:   [======    ]  60.0% (used 49128 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510632 bytes from 1023984 bytes)

RAM:   [======    ]  60.0% (used 49112 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510668 bytes from 1023984 bytes)

RAM:   [======    ]  59.8% (used 48952 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510492 bytes from 1023984 bytes)

RAM:   [======    ]  59.6% (used 48856 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510612 bytes from 1023984 bytes)

RAM:   [======    ]  59.6% (used 48856 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510652 bytes from 1023984 bytes)

RAM:   [======    ]  59.6% (used 48792 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510316 bytes from 1023984 bytes)

RAM:   [======    ]  59.3% (used 48552 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510620 bytes from 1023984 bytes)


RAM:   [======    ]  58.9% (used 48248 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510260 bytes from 1023984 bytes)

=====================================================================
*/


#ifdef USE_DECLARE_AT_COMPILE_TO_DEBUG
    mHardwarePins mod;
    mSettings mset;
    mSupport msup;
    mLogging mso;
    mTime mt;
    mMQTT mqt;
    mWiFi mwif;
    mTelemetry mtel;
    #ifdef USE_MODULE_CORE_WEBSERVER
    mWebServer mweb;
    #endif
  #ifdef USE_MODULE_SENSORS_DHT
    mSensorsDHT msdht;
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    mSensorsBME msbme;
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    mSensorsDB18 msdb18;
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    mUltraSonicSensor mus;
  #endif
  #ifdef USE_MODULE_CUSTOM_OILFURNACE
    mOilFurnace mof;
  #endif
  #ifdef USE_MODULE_CUSTOM_BLINDS
    mBlinds mbbl;
  #endif
  #ifdef   USE_MODULE_SENSORS_PULSE_COUNTER
    mPulseCounter mpc;
  #endif

  
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    mHBridge mdhb;
  #endif

  #ifdef USE_MODULE_DRIVERS_ENERGY
    mEnergy mdenergy;
  #endif
  /*
    Lights
    */
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    mInterfaceLight mil;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
    mRGBAnimator mrgbani;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_PWM
    mPWMLight mlights_pwm;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif



  #ifdef USE_MODULE_CUSTOM_HEATING
    mHeating mh;
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    mRelays mry;
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM  //this is really a pin class, but keep it on its own for now
    mPWM mpwm;
  #endif
  #ifdef USE_MODULE_CUSTOM_SECURITY_LIGHT
    mGarageLights mrl;
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    mMotionSensor mms;
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    mDoorSensor mds;
  #endif
  #ifdef USE_MODULE_SENSORS_DOORCHIME
    mDoorBell mdb;
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    mRadiatorFan mrf;
  #endif
  #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
    mSonoffIFan mifan;
  #endif
  #ifdef USE_MODULE_CUSTOM_FAN
    mFan mfan;
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    mIRtransceiver mir;
  #endif
  #if defined(USE_MODULE_DRIVERS_RF433MHZ) || defined (USE_MODULE_DRIVERS_RF433MHZ)
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
   mNextionPanel mnext;
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    mMoistureSensor mois;
  #endif

  // Sensors
  #ifdef USE_MODULE_SENSORS_BUTTONS
    mButtons mbtn;
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    mSwitches mswh;
  #endif
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    mPzem_AC mspm;
  #endif


  #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
    mExerciseBike meb;
  #endif
  
void init_class_instances(){
   pCONT->mod = &mod;
   pCONT->mset = &mset;
    pCONT-> msup = &msup;
    pCONT-> mso = &mso;
    pCONT-> mt = &mt;
    pCONT-> mqt = &mqt;
    pCONT-> mwif = &mwif;
    pCONT-> mtel = &mtel;
    #ifdef USE_MODULE_CORE_WEBSERVER
    pCONT-> mweb = &mweb;
    #endif
  #ifdef USE_MODULE_SENSORS_DHT
    pCONT-> msdht = &mset;
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    pCONT-> msbme = &mset;
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    pCONT-> msdb18;
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    pCONT-> mus;
  #endif
  #ifdef USE_MODULE_CUSTOM_OILFURNACE
    pCONT-> mof;
  #endif
  #ifdef USE_MODULE_CUSTOM_BLINDS
    pCONT-> mbbl;
  #endif
  #ifdef   USE_MODULE_SENSORS_PULSE_COUNTER
    pCONT-> mpc;
  #endif

  
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    pCONT-> mdhb;
  #endif

  #ifdef USE_MODULE_DRIVERS_ENERGY
    pCONT-> mdenergy;
  #endif
  /*
    Lights
    */
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    pCONT-> mil = &mil;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
    pCONT-> mrgbani = &mrgbani;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_PWM
    pCONT-> mlights_pwm;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif



  #ifdef USE_MODULE_CUSTOM_HEATING
    pCONT-> mh;
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    pCONT-> mry;
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM  //this is really a pin class, but keep it on its own for now
    pCONT-> mpwm;
  #endif
  #ifdef USE_MODULE_CUSTOM_SECURITY_LIGHT
    pCONT-> mrl;
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    pCONT-> mms;
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    pCONT-> mds;
  #endif
  #ifdef USE_MODULE_SENSORS_DOORCHIME
    pCONT-> mdb;
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    pCONT-> mrf;
  #endif
  #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
    pCONT-> mifan;
  #endif
  #ifdef USE_MODULE_CUSTOM_FAN
    pCONT-> mfan;
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    pCONT-> mir;
  #endif
  #if defined(USE_MODULE_DRIVERS_RF433MHZ) || defined (USE_MODULE_DRIVERS_RF433MHZ)
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
   pCONT-> mnext;
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    pCONT-> mois;
  #endif

  // Sensors
  #ifdef USE_MODULE_SENSORS_BUTTONS
    pCONT-> mbtn;
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    pCONT-> mswh;
  #endif
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    pCONT-> mspm;
  #endif


  #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
    pCONT-> meb;
  #endif

}

#endif


#ifdef ESP32
  // TMP placement
  // ESP32 specific
  //

  #include "soc/soc.h"
  #include "soc/rtc_cntl_reg.h"
  #define DISABLE_ESP32_BROWNOUT

  void DisableBrownout(void) {
    // https://github.com/espressif/arduino-esp32/issues/863#issuecomment-347179737
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
  }
#endif // ESP32

/********************************************************************************************/
/*********************SETUP******************************************************************/
/********************************************************************************************/


void setup(void)
{ 
  hw_wdt_disable();
  

  // Serial2.begin(115200);
  // Serial2.println("115200");

  // Serial.begin(115200);
  #ifndef USE_DEVFEATURE_DISABLE_ALL_PROJECT_FOR_TESTING
  #ifdef ESP32
  #ifdef DISABLE_ESP32_BROWNOUT
    DisableBrownout();      // Workaround possible weak LDO resulting in brownout detection during Wifi connection
  #endif
  #endif

  Serial.begin(115200);

  #ifdef USE_SERIAL_ALTERNATE_TX
    Serial.set_tx(2);
  #endif
  Serial.println(F("\n\rRebooting..." DEBUG_INSERT_PAGE_BREAK));
  #ifndef DISABLE_SERIAL_LOGGING
  #ifdef ENABLE_BUG_TRACING
  Serial.println(F("DELAYED BOOT for 5 seconds...")); Serial.flush(); delay(5000);
  #endif
  #endif


  // Init Json builder with memory address and size
  JsonBuilderI->Start(data_buffer.payload.ctr,&data_buffer.payload.len,DATA_BUFFER_PAYLOAD_MAX_LENGTH);
  BufferWriterI->Start(data_buffer.payload.ctr,&data_buffer.payload.len,DATA_BUFFER_PAYLOAD_MAX_LENGTH);

  // Configure WDT
  // msup.WDT_Begin();
  #ifdef USE_DECLARE_AT_COMPILE_TO_DEBUG
  void init_class_instances();
  #endif 
  
  pCONT->Instance_Init();

  // Set boot method
  pCONT_set->seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;
  
  // Clear boot status info
  memset(&pCONT_set->boot_status,0,sizeof(pCONT_set->boot_status));

  // All pointers must be set above
  int8_t result = pCONT->Tasker_Interface(FUNC_CHECK_POINTERS);

  if(result != FUNCTION_RESULT_SUCCESS_ID){
    delay(10000); // long enough to cause reset
  }  

  pCONT->InitClassList();
    
  pCONT_set->RtcRebootLoad();
  if(!pCONT_set->RtcRebootValid()) { pCONT_set->RtcReboot.fast_reboot_count = 0; }
  pCONT_set->RtcReboot.fast_reboot_count++;
  pCONT_set->RtcRebootSave();

  pCONT_sup->init_FirmwareVersion();
  
  // Load config from memory
  pCONT_set->SettingsDefault(); //preload minimal required
  #ifdef ENABLE_SETTINGS_STORAGE
  // Overwrite with latest values, including template if new SETTINGS_CONFIG exists
  pCONT_set->SettingsLoad();    //overwrite stored settings from defaults
  // Check Load was successful
  pCONT_set->SettingsLoad_CheckSuccessful();
  #endif
  
  #ifdef ESP8266  // ESP 32 can't start this process until a connection happens
  #ifdef USE_ARDUINO_OTA
    pCONT_sup->ArduinoOTAInit();
  #endif // USE_ARDUINO_OTA
  #endif
   
  pCONT->Tasker_Interface(FUNC_POINTER_INIT); // confirgure any memory address needed as part of module init or templates
  
  #ifdef FORCE_TEMPLATE_LOADING
  // This will overwrite the settings, temporary, will use a second flag to force template loads "TEMPLATE_HOLDER"
  // need to if template not provided, load defaults else use settings -- add protection in settings defaults to use templates instead (progmem or user desired)
  // Load template before init
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_WARN,PSTR(D_LOG_MEMORY D_LOAD " Temporary loading any progmem templates"));
    #endif
  pCONT->Tasker_Interface(FUNC_TEMPLATE_MODULE_LOAD); // loading module, only interface modules will have these
  // load
  // pCONT->Tasker_Interface(FUNC_TEMPLATE_DEVICE_LOAD);  //load/overwrite names AFTER init (FUNC_TEMPLATE_DEVICE_CONFIG_BEFORE_INIT)
   // Set boot method
  pCONT_set->seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;
  #endif

  // Init the GPIOs
  pCONT_pins->GpioInit();
  // Start pins in modules
  pCONT->Tasker_Interface(FUNC_PRE_INIT);
  // Init devices
  pCONT->Tasker_Interface(FUNC_INIT);
  // Init devices after others have been configured fully
  pCONT->Tasker_Interface(FUNC_POST_INIT);
  // Run system functions 
  pCONT->Tasker_Interface(FUNC_FUNCTION_LAMBDA_INIT);
  // Load the minimal default settings in modules (hard coded) before loading any stored user values
  pCONT->Tasker_Interface(FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES); // load the minimal
  // Load any stored user values into module
  pCONT->Tasker_Interface(FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE);
  // load
  pCONT->Tasker_Interface(FUNC_TEMPLATE_DEVICE_LOAD);         //load/overwrite names AFTER init (FUNC_TEMPLATE_DEVICE_CONFIG_AFTER_INIT)
  // Configure sensor/drivers to values desired for modules
  pCONT->Tasker_Interface(FUNC_CONFIGURE_MODULES_FOR_DEVICE);
  // init mqtt handlers from memory
  pCONT->Tasker_Interface(FUNC_MQTT_HANDLERS_INIT);
  
  #ifdef ENABLE_FUNCTION_DEBUG
    pCONT->Tasker_Interface(FUNC_DEBUG_CONFIGURE);
  #endif
  // Used to show progress of boot in logs
  pCONT->Tasker_Interface(FUNC_ON_SUCCESSFUL_BOOT);

  #endif // USE_DEVFEATURE_DISABLE_ALL_PROJECT_FOR_TESTING
#ifdef USE_DEVFEATURE_UART_TEST
  // UART_Setup();
  Serial.begin(115200);
  Serial.println("Booting UART0");

  // delay(3000);

  //   buffer_queue = xQueueCreate( 10, sizeof( buffer_test ) );
 	//   if( buffer_queue == 0 )
  // 	{
  //     // Serial.println();
  //     Serial.println("buffer_queue FAILED!"); delay(4000);
  // 		// Failed to create the queue.
  //     memset(&buffer_test,0,sizeof(buffer_test));
  //     memcpy(buffer_test, "failed", sizeof("failed")-1);
  // 	}else{
  //     Serial.println("buffer_queue SUCCESS!"); delay(4000);

  //     memset(&buffer_test,0,sizeof(buffer_test));
  //     memcpy(buffer_test, "success", sizeof("success")-1);
  //   }


    buf_handle = xRingbufferCreate(BUF_HANDLE_LENGTH, RINGBUF_TYPE_BYTEBUF);
    if (buf_handle == NULL) {
      Serial.printf("Failed to create ring buffer\n\r");
    }else{
      
      Serial.printf("SUCCESS to create ring buffer\n\r");
    }


    char message[20] = {0};

    for(int ii=0;ii<100;ii++){

      sprintf(message,"message%02d",ii);
      UBaseType_t res =  xRingbufferSend(buf_handle, message, strlen(message), pdMS_TO_TICKS(1000));
      if (res != pdTRUE) {
        Serial.printf("Failed to send item\n\r");
      }else{
        // Serial.printf("messaged added\n\r");
      }

    }




#endif // USE_DEVFEATURE_UART_TEST

}


// /********************************************************************************************/
// /*********************loop*******************************************************************/
// /********************************************************************************************/

uint32_t ticktest = millis();
uint32_t tick_fill = millis();
uint32_t tick_read = millis();
uint16_t number = 0;
uint16_t  freeSize = 0;

// Test case
// Write into the buffer, every 5 seconds
// only read from it every 30 seconds
// print entire buffer out every 5 seconds

void loop(void)
{
  yield();
#ifdef USE_DEVFEATURE_UART_TEST
  // UART_Loop();

  // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos_additions.html

/**
 * @brief   Get information about ring buffer status
 *
 * Get information of the a ring buffer's current status such as
 * free/read/write pointer positions, and number of items waiting to be retrieved.
 * Arguments can be set to NULL if they are not required.
 *
 * @param[in]   xRingbuffer     Ring buffer to remove from the queue set
 * @param[out]  uxFree          Pointer use to store free pointer position
 * @param[out]  uxRead          Pointer use to store read pointer position
 * @param[out]  uxWrite         Pointer use to store write pointer position
 * @param[out]  uxItemsWaiting  Pointer use to store number of items (bytes for byte buffer) waiting to be retrieved
 */
// void vRingbufferGetInfo(RingbufHandle_t xRingbuffer, UBaseType_t *uxFree, UBaseType_t *uxRead, UBaseType_t *uxWrite, UBaseType_t *uxItemsWaiting);
// RingbufHandle_t xRingbuffer;
// UBaseType_t *uxFree;
// UBaseType_t *uxRead;
// UBaseType_t *uxWrite;
// UBaseType_t *uxItemsWaiting;
// vRingbufferGetInfo(xRingbuffer, uxFree, uxRead, uxWrite, uxItemsWaiting);

// #ifdef ENABLE_DEVFEATURE_BYTEBUFFER
  if(abs(millis() - tick_fill)>1000){ tick_fill = millis();
    Serial.println("Writing into buffer");

    char message[100] = {0};
    for(int i=0;i<9;i++){
      sprintf(message+strlen(message),"%d",number);
    }
    number++;

    UBaseType_t res =  xRingbufferSend(buf_handle, message, strlen(message), pdMS_TO_TICKS(1000));
    if (res != pdTRUE) {
      Serial.printf("Failed to send item\n\r");
    }else{
      // Serial.printf("messaged added\n\r");
    }
    
  //   // freeSize = xRingbufferGetCurFreeSize(buf_handle);
  //   // Serial.printf("freeSize1=%d\n\r",freeSize);

  //   // UBaseType_t res2 =  xRingbufferSend(buf_handle, message, sizeof(message), pdMS_TO_TICKS(1000));
  //   // if (res2 != pdTRUE) {
  //   //   Serial.printf("Failed to send item\n\r");
  //   // }else{
  //   //   // Serial.printf("messaged added\n\r");
  //   // }

  //   // freeSize = xRingbufferGetCurFreeSize(buf_handle);
  //   // Serial.printf("freeSize2=%d\n\r",freeSize);


  //   //  for(int i=0;i<10;i++){
  //   //   sprintf(message+strlen(message),"a",number);
  //   // }
  //   // res =  xRingbufferSend(buf_handle, message, 
  //   // //sizeof(tx_item), 
  //   // sizeof(message),
  //   // pdMS_TO_TICKS(1000));
  //   // if (res != pdTRUE) {
  //   //   Serial.printf("Failed to send item\n\r");
  //   // }else{
  //   //   // Serial.printf("messaged added\n\r");
  //   // }
    
  //   // Serial.printf("buff>>%s",bu);

  }


// #endif // test

  if(abs(millis() - tick_read)>5000){ tick_read = millis();
    //Receive an item from allow-split ring buffer

    xRingbufferPrintInfo(buf_handle);
    //Receive an item from no-split ring buffer
    size_t item_size;
    char *item = (char *)xRingbufferReceive(buf_handle, &item_size, pdMS_TO_TICKS(1000));

    //Check received item
    if (item != NULL) {
        //Print item
        for (int i = 0; i < item_size; i++) {
            printf("%c", item[i]);
        }
        printf("\n");
        //Return Item
        vRingbufferReturnItem(buf_handle, (void *)item);
    } else {
        //Failed to receive item
        printf("Failed to receive item\n");
    }


    // while(
    //   (freeSize = xRingbufferGetCurFreeSize(buf_handle))
    //   <
    //   984
    //   ){
    
    //   Serial.printf("freeSize while = %d\n\r",freeSize);

    //   if(abs(millis() - tick_read)>10000){
    //     break;
    //   }
      
    //   size_t item_size1, item_size2;
    //   char *item1, *item2;
    //   BaseType_t ret = xRingbufferReceiveSplit(buf_handle, (void **)&item1, (void **)&item2, &item_size1, &item_size2, pdMS_TO_TICKS(1000));

    //   //Check received item
    //   if (ret == pdTRUE && item1 != NULL) {
    //     Serial.printf("item1 size = %d\n\r", item_size1);
    //     for (int i = 0; i < item_size1; i++) {
    //       Serial.printf("%c", item1[i]);
    //     }
    //     vRingbufferReturnItem(buf_handle, (void *)item1);
    //     //Check if item was split
    //     if (item2 != NULL) {
    //       for (int i = 0; i < item_size2; i++) {
    //         Serial.printf("%c", item2[i]);
    //       }
    //       vRingbufferReturnItem(buf_handle, (void *)item2);
    //     }
    //     Serial.printf("\n");
    //   } else {
    //     //Failed to receive item
    //     Serial.printf("Failed to receive item\n");
    //   }

// xRingbufferPrintInfo(buf_handle);
  
    }
    // read half out

    //print whole buffer
    // for(int ii=0;ii<100;ii++){
    //   Serial.print((char)buffer_test[ii]);
    // }
    // Serial.println();

  // }


#endif // USE_DEVFEATURE_UART_TEST







  #ifndef USE_DEVFEATURE_DISABLE_ALL_PROJECT_FOR_TESTING
  DEBUG_LINE;
  pCONT_sup->activity.loop_counter++;
  pCONT_sup->loop_start_millis = millis();
  WDT_RESET();

  #ifdef DEVICE_SIDEDOORLIGHT
  pinMode(5, INPUT);
  #endif
  
  DEBUG_LINE;
  
  #ifdef USE_ARDUINO_OTA
    pCONT_sup->ArduinoOtaLoop();
  #endif  // USE_ARDUINO_OTA
  
  DEBUG_LINE;
  pCONT->Tasker_Interface(FUNC_LOOP); // EVERY_LOOP

  DEBUG_LINE;
  if(pCONT_time->uptime.seconds_nonreset > 30){ pCONT->Tasker_Interface(FUNC_FUNCTION_LAMBDA_LOOP); } // Only run after stable boot

 
  //move into support, or into time, to align with every_minute, hour, etc
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop50mSec ,50  )){ pCONT->Tasker_Interface(FUNC_EVERY_50_MSECOND);  }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop100mSec,100 )){ pCONT->Tasker_Interface(FUNC_EVERY_100_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop200mSec,200 )){ pCONT->Tasker_Interface(FUNC_EVERY_200_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop250mSec,250 )){ pCONT->Tasker_Interface(FUNC_EVERY_250_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop1Sec   ,1000)){ pCONT->Tasker_Interface(FUNC_EVERY_SECOND);  
  
  // Serial.println(STA_PASS1);
   };// DEBUG_LINE_HERE;   } 
       
  /********************************************************************************************************
  ******************************************************************************************************** 
    End of loop, only calculate or sleep after this point
  ********************************************************************************************************
  ********************************************************************************************************/
    
  pCONT_sup->loop_runtime_millis = millis() - pCONT_sup->loop_start_millis;

  DEBUG_LINE;

  if(mTime::TimeReached(&pCONT_set->runtime_value.tSavedUpdateLoopStatistics, 1000)){
    pCONT_sup->activity.cycles_per_sec = pCONT_sup->activity.loop_counter; 
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("LOOPSEC = %d %d"), pCONT_sup->activity.loop_counter, pCONT_sup->activity.cycles_per_sec);
    #endif // ENABLE_LOG_LEVEL_INFO
    pCONT_sup->activity.loop_counter=0;
  }

  if(pCONT_sup->loop_runtime_millis > 40){
    Serial.printf("loop_runtime_millis=%d\n\r", pCONT_sup->loop_runtime_millis);
  }

  //  pCONT_mqtt->flag_uptime_reached_reduce_frequency = true;

  // Change this to my own way
  // DO THIS NEXT
  //SmartLoopDelay()
  #ifndef DISABLE_SLEEP
  if(pCONT_set->Settings.enable_sleep){
    if (pCONT_set->Settings.flag_network.sleep_normal) {
      pCONT_sup->SleepDelay(pCONT_set->runtime_value.sleep);
    } else {

      // Loop time < sleep length of time
      if (pCONT_sup->loop_runtime_millis < (uint32_t)pCONT_set->runtime_value.sleep) {
        //delay by loop time
        pCONT_sup->SleepDelay((uint32_t)pCONT_set->runtime_value.sleep - pCONT_sup->loop_runtime_millis);  // Provide time for background tasks like wifi
      } else {

        // if loop takes longer than sleep period, no delay, IF wifi is down, devote half loop time to wifi connect
        if (pCONT_set->global_state.wifi_down) {
          pCONT_sup->SleepDelay(pCONT_sup->loop_runtime_millis /2); // If wifi down and loop_runtime_millis > setoption36 then force loop delay to 1/3 of loop_runtime_millis period
        }
      }
    }
  }
  #endif

  // delay(100);

  DEBUG_LINE;
  if (!pCONT_sup->loop_runtime_millis) { pCONT_sup->loop_runtime_millis++; }            // We cannot divide by 0
  pCONT_sup->loop_delay_temp = pCONT_set->runtime_value.sleep; 
  if (!pCONT_sup->loop_delay_temp) { pCONT_sup->loop_delay_temp++; }              // We cannot divide by 0
  pCONT_sup->loops_per_second = 1000 / pCONT_sup->loop_delay_temp;  // We need to keep track of this many loops per second, 20ms delay gives 1000/20 = 50 loops per second (50hz)
  pCONT_sup->this_cycle_ratio = 100 * pCONT_sup->loop_runtime_millis / pCONT_sup->loop_delay_temp;
  pCONT_set->loop_load_avg = pCONT_set->loop_load_avg - (pCONT_set->loop_load_avg / pCONT_sup->loops_per_second) + (pCONT_sup->this_cycle_ratio / pCONT_sup->loops_per_second); // Take away one loop average away and add the new one

  DEBUG_LINE;
  // Create a debug mqtt packet for timings, of main loop and interface loops
  // Serial.printf("%s=%d\r\n","tick",pCONT_sup->loop_runtime_millis);
  // Serial.printf("%s=%d\r\n","tick",pCONT_sup->activity.cycles_per_sec);
  // uint32_t start_millis = millis();
  // AddLog_P(LOG_LEVEL_TEST,PSTR("LOOPSEC = %d \t %d"),loops_per_second,mtel.activity.cycles_per_sec);
  // Serial.printf("ADD TIME = %d\n\r",millis()-start_millis);

  pCONT_set->fSystemRestarted = false; //phase out and use module flag instead

  DEBUG_LINE;

  #endif // USE_DEVFEATURE_DISABLE_ALL_PROJECT_FOR_TESTING


}





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
    
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    init_UART0();
    init_UART1();
    init_UART2();


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

// 	/* Configure parameters of an UART driver,
// 	* communication pins and install the driver */
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

	/* Configure parameters of an UART driver,
	* communication pins and install the driver */
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

	/* Configure parameters of an UART driver,
	* communication pins and install the driver */
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
#define UART2_METHOD_QUEUE

void init_UART2(){

#ifdef UART2_METHOD_NO_QUEUE
	// esp_log_level_set(TAG, ESP_LOG_INFO);

	/* Configure parameters of an UART driver,
	* communication pins and install the driver */
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

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
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
    
    	uart_queue = xQueueCreate( 10, sizeof( struct AMessage * ) );
 	  if( uart_queue == 0 )
  	{
      // Serial.println("uart_queue FAILED!"); delay(4000);
  		// Failed to create the queue.
  	}
      // Serial.println("uart_queue SUCESS!"); delay(4000);

    	uart0_queue = xQueueCreate( 10, sizeof( struct AMessage * ) );
 	  if( uart0_queue == 0 )
  	{
      // Serial.println("uart_queue FAILED!"); delay(4000);
  		// Failed to create the queue.
  	}
      // Serial.println("uar/t_queue SUCESS!"); delay(4000);

    
    // xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
#endif // UART2_METHOD_QUEUE




  // uart_write_bytes(
  //  UART_NUM_2,
  //  (const char*) "UART2 - Reboot\n\r", 17);

  
}


void UART_Loop(){

  // Serial.println("loop");

  if(urxlen){
    for(int ii=0;ii<urxlen;ii++){
      Serial.print((char)rxbuf[ii]);
    }
    Serial.println();
    urxlen = 0;
  }


}


#endif // UART_TEST


