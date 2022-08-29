#include "mSerialUART.h"

#ifdef USE_MODULE_DRIVERS_SERIAL_UART

#ifdef ENABLE_HARDWARE_UART_1

void mSerialUART::init_UART1_RingBuffer()
{

  settings.uart1.ringbuffer_handle = xRingbufferCreate(settings.uart1.ring_buffer_size_rx, RINGBUF_TYPE_BYTEBUF);
  if (settings.uart1.ringbuffer_handle != NULL) {
    settings.uart1.initialised = true;
  }

  

  char data_ctr[] = "UART1 RingBuf Init\0";
  UBaseType_t res =  xRingbufferSend(settings.uart1.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
  if (res != pdTRUE) {
    AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
    settings.uart1.initialised = false; //disable if false
  }else{

    AddLog(LOG_LEVEL_ERROR, PSTR("%s SUCCESS"),data_ctr);


  }
// assert(settings.uart1.ringbuffer_handle != NULL);
  

}

void mSerialUART::init_UART1_pins()
{

  /* Configure parameters of an UART driver, communication pins and install the driver */
  uart_config_t uart_config = {
    .baud_rate = settings.uart1.baud,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };

  ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
  //Set UART gpio
	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, settings.uart1.gpio.tx, settings.uart1.gpio.rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

}

/**
 * @brief init UART1 ISR routine
 * @note
 * @param void none
 * @return none
 */
void mSerialUART::init_UART1_ISR(){

  AddLog(LOG_LEVEL_DEBUG, PSTR("init_UART1_ISR Starting..."));

  // If serial2 has already been activated by a library, disable it first so the new driver can attach
  Serial1.end();  

  init_UART1_pins(); //reassert new baud

  // Install UART driver, and get the queue.
  ESP_ERROR_CHECK(
    uart_driver_install(
      UART_NUM_1,                           // UART Number
      settings.uart1.ring_buffer_size_rx,   // UART RX ring buffer size.
      settings.uart1.ring_buffer_size_tx,   // UART TX ring buffer size. If set to zero, driver will not use TX buffer, TX function will block task until all data have been sent out.
      settings.uart1.event_queue_size,      // UART event queue size/depth.
     &settings.uart1.event_queue_handle,    // UART event queue handle (out param). On success, a new queue handle is written here to provide access to UART events. If set to NULL, driver will not use an event queue.
      0                                     // intr_alloc_flags (esp_intr_alloc.h)
    )
  );

	// // release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(
    uart_isr_free(UART_NUM_1)       // Free UART interrupt handler registered by uart_isr_register. Must be called on the same core as uart_isr_register was called.
  ); 

  uart_isr_register(            // UART ISR handler will be attached to the same CPU core that this function is running on.
    UART_NUM_1,                 // UART_NUM_0, UART_NUM_1 or UART_NUM
    UART1_ISR_Static, // Interrupt handler function.
    NULL,                       // parameter for handler function
    ESP_INTR_FLAG_IRAM,         // Flags used to allocate the interrupt. One or multiple (ORred) ESP_INTR_FLAG_* values. See esp_intr_alloc.h for more info. 
    uart1_handle_console        // Pointer to return handle. If non-NULL, a handle for the interrupt will be returned here.
  );

  AddLog(LOG_LEVEL_DEBUG, PSTR(DEBUG_INSERT_PAGE_BREAK "init_UART1_ISR Started %d buffer size"),settings.uart1.ring_buffer_size_rx);

  #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
  gpio_pad_select_gpio(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(BLINK_GPIO, LOW); // Turn off by default
  #endif
// esp_log_level_set("*", ESP_LOG_VERBOSE);
// esp_log_level_set(TAG, ESP_LOG_INFO);


// std::cout << "x" << std::flush;
  
//     ESP_LOGI(TAG, "Initializing SD card");

}


#define UART1_ISR_TMP_BUFFER_SIZE 5000
// Receive buffer to collect incoming data
uint8_t rxbuf1[UART1_ISR_TMP_BUFFER_SIZE] = {0};
// Register to collect data length
uint16_t urxlen1 = 0;

/**
 * @brief Define UART interrupt subroutine to ackowledge interrupt
 * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
 **/
void IRAM_ATTR UART1_ISR_Static(void *arg)
{ 

// gps data

 // DEBUG_PIN1_SET(LOW);
  
  uint16_t rx_fifo_len, status;
  uint16_t i = 0;

  status = UART1.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART1.status.rxfifo_cnt; // read number of bytes in UART buffer

  while(UART1.status.rxfifo_cnt)
  {
    rxbuf1[i++] = UART1.fifo.rw_byte; // You can not directly access the UART0.fifo.rw_byte on esp31s1 but have to use READ_PERI_REG(UART_FIFO_AHB_REG(0))
    if(i>=UART1_ISR_TMP_BUFFER_SIZE-1)
    {
      break;
    }
    rx_fifo_len--;
  }
  urxlen1 = i;

  BaseType_t dummyval;
  UBaseType_t res =  xRingbufferSendFromISR(pCONT_uart->settings.uart1.ringbuffer_handle, rxbuf1, urxlen1, &dummyval);
  
  // after reading bytes from buffer clear UART interrupt status
  uart_clear_intr_status(UART_NUM_1, 
    UART_RXFIFO_TOUT_INT_CLR |
    // UART_BRK_DET_INT_CLR |
    // UART_CTS_CHG_INT_CLR |
    // UART_DSR_CHG_INT_CLR |
    // UART_RXFIFO_OVF_INT_CLR |
    UART_FRM_ERR_INT_CLR | // UART Frame Error
    // UART_PARITY_ERR_INT_CLR |
    // UART_TXFIFO_EMPTY_INT_CLR |
    UART_RXFIFO_FULL_INT_CLR 
  );


  #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
  gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
  #endif

 // DEBUG_PIN1_SET(HIGH);
}


#endif // ENABLE_HARDWARE_UART_1



#endif
