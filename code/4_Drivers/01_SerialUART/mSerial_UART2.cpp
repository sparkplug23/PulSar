#include "mSerialUART.h"

#ifdef USE_MODULE_DRIVERS_SERIAL_UART



#ifdef ENABLE_HARDWARE_UART_2

char uart2_last_byte = 0x00;
uint16_t uart2_byte_index = 0;


#define UART2_ISR_TMP_BUFFER_SIZE 500
// Receive buffer to collect incoming data
uint8_t rxbuf2[UART2_ISR_TMP_BUFFER_SIZE] = {0};
// Register to collect data length
uint16_t urxlen2 = 0;

  // uint8_t uart_rss_tmp_buf_writer_index = 0;
  // char uart_rss_tmp_buffer_0[300] = {0};
  // uint16_t uart_rss_tmp_buflen_0 = 0;
  // char uart_rss_tmp_buffer_1[300] = {0};
  // uint16_t uart_rss_tmp_buflen_1 = 0;
  // char uart_rss_tmp_buffer_2[300] = {0};
  // uint16_t uart_rss_tmp_buflen_2 = 0;
    //  char special_json_part_of_gps_buffer[300]  = {0};
    //  uint16_t special_json_part_of_gps_buflen = 0;



void mSerialUART::init_UART2_RingBuffer()
{

  #ifdef ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
  settings.uart2.ringbuffer_format_type = RINGBUF_TYPE_NOSPLIT;
  #else
  settings.uart2.ringbuffer_format_type = RINGBUF_TYPE_BYTEBUF;
  #endif

  // Serial.printf("heap size1 = %d\n\r", ESP.getHeapSize()); Serial.flush();
  DEBUG_LINE_HERE;

  settings.uart2.ringbuffer_handle = xRingbufferCreate(settings.uart2.ring_buffer_size_rx, settings.uart2.ringbuffer_format_type);
  if (settings.uart2.ringbuffer_handle != NULL) {
    settings.uart2.initialised = true;
  // Serial.printf("settings.uart2.initialised = true;\n\r"); Serial.flush();
  }else{
    
  // Serial.printf("settings.uart2.initialised = false;\n\r"); Serial.flush();
  }

  if(settings.uart2.ringbuffer_handle == NULL)
  {
  // Serial.printf("settings.uart2.ringbuffer_handle == NULL\n\r"); Serial.flush();

  }

  // Serial.printf("heap size2 = %d\n\r", ESP.getHeapSize()); Serial.flush();
  DEBUG_LINE_HERE;
  // char data_ctr[] = "UART2 RingBuf Init\0";
  // UBaseType_t res =  xRingbufferSend(settings.uart2.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
  // if (res != pdTRUE) {
  //   AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
  //   settings.uart2.initialised = false; //disable if false
  // }

  
  #ifdef ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
  
  
  char data_ctr[] = "UART2 RingBuf Init\0";
  UBaseType_t res =  xRingbufferSend(settings.uart2.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
  if (res != pdTRUE) {
    AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
    settings.uart2.initialised = false; //disable if false
  }else{
    
    AddLog(LOG_LEVEL_TEST, PSTR("%s SUCCESS"),data_ctr);
  }

  assert(settings.uart2.ringbuffer_handle != NULL);



  #else
  
    char data_ctr[] = "UART2 RingBuf Init\0";
  UBaseType_t res =  xRingbufferSend(settings.uart2.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
  if (res != pdTRUE) {
    AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
    settings.uart2.initialised = false; //disable if false
  }


  #endif




}


/**
 * split from other ISR init so pins can be configured without attaching the interrupt
 * */
void mSerialUART::init_UART2_pins()
{

  /* Configure parameters of an UART driver, communication pins and install the driver */
  uart_config_t uart_config = {
    .baud_rate = settings.uart2.baud,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };

  ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
  //Set UART gpio
	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, settings.uart2.gpio.tx, settings.uart2.gpio.rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

  ESP_ERROR_CHECK(uart_set_baudrate(UART_NUM_2, settings.uart2.baud));

}



/**
 * @brief init UART2 ISR routine
 * @note
 * @param void none
 * @return none
 */
void mSerialUART::init_UART2_ISR(){

  AddLog(LOG_LEVEL_DEBUG, PSTR("init_UART2_ISR Starting..."));

  // If serial2 has already been activated by a library, disable it first so the new driver can attach
  Serial2.end();
  // Serial2.begin(2048000);
  
  init_UART2_pins(); //reassert new baud

  // Install UART driver, and get the queue.
  ESP_ERROR_CHECK(
    uart_driver_install(
      UART_NUM_2,                           // UART Number
      settings.uart2.ring_buffer_size_rx,   // UART RX ring buffer size.
      settings.uart2.ring_buffer_size_tx,   // UART TX ring buffer size. If set to zero, driver will not use TX buffer, TX function will block task until all data have been sent out.
      settings.uart2.event_queue_size,      // UART event queue size/depth.
     &settings.uart2.event_queue_handle,    // UART event queue handle (out param). On success, a new queue handle is written here to provide access to UART events. If set to NULL, driver will not use an event queue.
      0                                     // intr_alloc_flags (esp_intr_alloc.h)
    )
  );

	// // release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(
    uart_isr_free(UART_NUM_2)       // Free UART interrupt handler registered by uart_isr_register. Must be called on the same core as uart_isr_register was called.
  ); 
  
  uart_isr_register(            // UART ISR handler will be attached to the same CPU core that this function is running on.
    UART_NUM_2,                 // UART_NUM_0, UART_NUM_1 or UART_NUM
  // #ifdef ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
    // UART2_ISR_Static_NoSplitRingBuffer, // Interrupt handler function.
    #ifndef USE_DEVFEATURE_MEASUREMENT_SYSTEM_CALIBRATION_METHOD_1
    UART2_ISR_Static_NoSplitRingBuffer_ForMeasurements, // Interrupt handler function.
    #else
    UART2_ISR_Static_NoSplitRingBuffer_ForMeasurements_Calibration, // Interrupt handler function.
    #endif
  // #else // ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
  //   UART2_ISR_Static_ByteRingBuffer, // Interrupt handler function.
  // #endif // ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
    NULL,                       // parameter for handler function
    ESP_INTR_FLAG_IRAM,         // Flags used to allocate the interrupt. One or multiple (ORred) ESP_INTR_FLAG_* values. See esp_intr_alloc.h for more info. 
    uart2_handle_console        // Pointer to return handle. If non-NULL, a handle for the interrupt will be returned here.
  );
  
  AddLog(LOG_LEVEL_DEBUG, PSTR(DEBUG_INSERT_PAGE_BREAK "init_UART2_ISR RSS Started %d buffer size"),settings.uart2.ring_buffer_size_rx);

  #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
  gpio_pad_select_gpio(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(BLINK_GPIO, LOW); // Turn off by default
  #endif

  
  // gpio_pad_select_gpio(GPIO_NUM_21);
  // gpio_set_direction(GPIO_NUM_21, GPIO_MODE_OUTPUT);
  // gpio_set_level(GPIO_NUM_21, LOW); // Turn off by default

}



// /**
//  * @brief Define UART interrupt subroutine to ackowledge interrupt
//  * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
//  **/
// void IRAM_ATTR UART2_ISR_Static_ByteRingBuffer(void *arg)
// { 
// //  DEBUG_PIN1_SET(LOW);
  
//   uint16_t rx_fifo_len, status;
//   uint16_t i = 0;

//   status = UART2.int_st.val; // read UART interrupt Status
//   rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer

//   while(rx_fifo_len)
//   {
//     pCONT_uart->rxbuf2[i++] = UART2.fifo.rw_byte; // You can not directly access the UART0.fifo.rw_byte on esp32s2 but have to use READ_PERI_REG(UART_FIFO_AHB_REG(0))
//     if(i>=RINGBUFFER_HANDLE_2_LENGTH-1)
//     {
//       break;
//     }
//     rx_fifo_len--;
//   }
//   pCONT_uart->urxlen2 = i;

//   BaseType_t dummyval;
//   UBaseType_t res =  xRingbufferSendFromISR(pCONT_uart->settings.uart2.ringbuffer_handle, pCONT_uart->rxbuf2, pCONT_uart->urxlen2, &dummyval);
  
//   // after reading bytes from buffer clear UART interrupt status
//   uart_clear_intr_status(UART_NUM_2, 
//     UART_RXFIFO_TOUT_INT_CLR |
//     // UART_BRK_DET_INT_CLR |
//     // UART_CTS_CHG_INT_CLR |
//     // UART_DSR_CHG_INT_CLR |
//     // UART_RXFIFO_OVF_INT_CLR |
//     UART_FRM_ERR_INT_CLR | // UART Frame Error
//     // UART_PARITY_ERR_INT_CLR |
//     // UART_TXFIFO_EMPTY_INT_CLR |
//     UART_RXFIFO_FULL_INT_CLR 
//   );


//   #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
//   gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//   #endif

//  // DEBUG_PIN1_SET(HIGH);
// }

// char conversion_buffer[500];
// uint16_t conversion_buflen = 0;


/**
 * @brief Define UART interrupt subroutine to ackowledge interrupt
 * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
 * 
 * Arriving data will be read (appended) to a global small buffer, each byte in will be checked. 
 * If special EOF chars are found, then the data in the global buffer is commited to the ringbuffer nosplit, which allows retreiving one item at a time
 * 
 * 
 * METHOD AUTOMATICALLY CREATES ARRAY FOR QUICK MOVE INTO JSON
 * 
 **/
void IRAM_ATTR UART2_ISR_Static_NoSplitRingBuffer_ForMeasurements(void *arg)
{ 
//  DEBUG_PIN1_SET(LOW);
  // gpio_set_level(GPIO_NUM_21, 0);
  
  uint16_t rx_fifo_len, status;
  // uint16_t i = 0;
  char byte_read = 0;
  bool flag_end_of_frame_detected = false;

  status = UART2.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer

  while(rx_fifo_len)
  {

    byte_read = UART2.fifo.rw_byte; // You can not directly access the UART0.fifo.rw_byte on esp32s2 but have to use READ_PERI_REG(UART_FIFO_AHB_REG(0))

    rxbuf2[uart2_byte_index++] = byte_read;//(char)uart2_byte_index;//byte_read;
    if(uart2_byte_index>=UART2_ISR_TMP_BUFFER_SIZE-1)
    {
      uart2_byte_index = 0; // forced reset
      // We should never reach here, unless no EOF are detected then wrapping around will dump/lose all data
      break;
    }

    /**
     * Check if this byte and last byte are EOF chars
     * */
    if((byte_read==0xFF)&&(uart2_last_byte==0xFF))
    {
      flag_end_of_frame_detected = true;
      break;
    }

    // rxbuf2[uart2_byte_index++] = ','; // Keep adding comma's only if needed

    // Update last byte, with anticaption of being read later
    uart2_last_byte = byte_read; //update with this recent byte

    rx_fifo_len--;
  }
  urxlen2 = uart2_byte_index;

  /**
   * if EOF is detected, commit data into nosplit buffer
   * */
  if(flag_end_of_frame_detected)
  {
    #ifdef ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER

    // conversion_buflen = 0;
    //   // conversion_buflen += sprintf(conversion_buffer+conversion_buflen, "%d:", millis());
    // for(int i=0;i<urxlen2;i++)
    // {
    //   conversion_buflen += sprintf(conversion_buffer+conversion_buflen, "%d,", rxbuf2[i]);
    // }
    // conversion_buflen-- ; // forget last comma



//     pCONT_uart->special_json_part_of_gps_buffer[0] = 253;
//  pCONT_uart->special_json_part_of_gps_buflen = 1;
// if(urxlen2){
  BaseType_t dummyval;

  // use start and end of array "~~" so matlab can search and repair the json
      xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle, "@{\"F\":[[A", 9, &dummyval);
      xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle, rxbuf2, urxlen2-2, &dummyval); //dont send two EOL bytes
      // xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle, conversion_buffer, conversion_buflen, &dummyval);
      // if(pCONT_uart->special_json_part_of_gps_buflen)
      // {
      //   xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle,  pCONT_uart->special_json_part_of_gps_buffer,  pCONT_uart->special_json_part_of_gps_buflen, &dummyval);
      // }
      // xRingbufferSend(pCONT_sdcard->stream.ringbuffer_handle, "]}\n\r", 4, pdMS_TO_TICKS(2));
// }

      // /**
      //  * New method, must write to a few circler buffers that is meant for sdcard
      //  * */
      // switch(uart_rss_tmp_buf_writer_index)
      // {
      //   case 0:
      //     // rxbuf2[0] = 2;
      //     if(uart_rss_tmp_buflen_0 == 0) // check if its empty
      //     {
      //       uart_rss_tmp_buflen_0 = urxlen2;
      //       memcpy(uart_rss_tmp_buffer_0, rxbuf2, urxlen2);
      //       uart_rss_tmp_buf_writer_index = 1;
      //     }
      //   break;
      //   case 1:
      //     // rxbuf2[0] = 3;
      //     if(uart_rss_tmp_buflen_1 == 0) // check if its empty
      //     {
      //       uart_rss_tmp_buflen_1 = urxlen2;
      //       memcpy(uart_rss_tmp_buffer_1, rxbuf2, urxlen2);
      //       uart_rss_tmp_buf_writer_index = 0;
      //     }
      //   break;
      //   // case 2:
      //   //   // rxbuf2[0] = 3;
      //   //   uart_rss_tmp_buflen_2 = urxlen2;
      //   //   memcpy(uart_rss_tmp_buffer_2, rxbuf2, urxlen2);
      //   //   uart_rss_tmp_buf_writer_index = 0;
      //   // break;
      // }


    #else
    BaseType_t dummyval;
    UBaseType_t res =  xRingbufferSendFromISR(pCONT_uart->settings.uart2.ringbuffer_handle, rxbuf2, urxlen2, &dummyval); // instead of ringbuffer, this probably also needs to be cycling buffers
    #endif // ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER

    /**
     * Reset byte index for next frame
     * */
    uart2_byte_index = 0;
  }
  
  // after reading bytes from buffer clear UART interrupt status
  uart_clear_intr_status(UART_NUM_2, 
    UART_RXFIFO_TOUT_INT_CLR |
    //clearing more, maybe its the cause of crashes?
    UART_BRK_DET_INT_CLR |
    //clearing more, maybe its the cause of crashes?
    UART_CTS_CHG_INT_CLR |
    //clearing more, maybe its the cause of crashes?
    UART_DSR_CHG_INT_CLR |
    //clearing more, maybe its the cause of crashes?
    UART_RXFIFO_OVF_INT_CLR |
    UART_FRM_ERR_INT_CLR | // UART Frame Error
    //clearing more, maybe its the cause of crashes?
    UART_PARITY_ERR_INT_CLR |
    UART_TXFIFO_EMPTY_INT_CLR | // Needed to enable transmitting on serial
    UART_RXFIFO_FULL_INT_CLR 
  );


  #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
  gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
  #endif
  // gpio_set_level(GPIO_NUM_21, 1);

//  DEBUG_PIN1_SET(HIGH);
}


#ifdef USE_DEVFEATURE_MEASUREMENT_SYSTEM_CALIBRATION_METHOD_1
/**
 * @brief Define UART interrupt subroutine to ackowledge interrupt
 * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
 * 
 * Arriving data will be read (appended) to a global small buffer, each byte in will be checked. 
 * If special EOF chars are found, then the data in the global buffer is commited to the ringbuffer nosplit, which allows retreiving one item at a time
 * 
 * 
 * METHOD AUTOMATICALLY CREATES ARRAY FOR QUICK MOVE INTO JSON
 * 
 **/
void IRAM_ATTR UART2_ISR_Static_NoSplitRingBuffer_ForMeasurements_Calibration(void *arg)
{ 
//  DEBUG_PIN1_SET(LOW);
  // gpio_set_level(GPIO_NUM_21, 0);
  
  uint16_t rx_fifo_len, status;
  // uint16_t i = 0;
  char byte_read = 0;
  bool flag_end_of_frame_detected = false;

  status = UART2.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer

  while(rx_fifo_len)
  {

    byte_read = UART2.fifo.rw_byte; // You can not directly access the UART0.fifo.rw_byte on esp32s2 but have to use READ_PERI_REG(UART_FIFO_AHB_REG(0))

    rxbuf2[uart2_byte_index++] = byte_read;//(char)uart2_byte_index;//byte_read;
    if(uart2_byte_index>=UART2_ISR_TMP_BUFFER_SIZE-1)
    {
      uart2_byte_index = 0; // forced reset
      // We should never reach here, unless no EOF are detected then wrapping around will dump/lose all data
      break;
    }

    /**
     * Check if this byte and last byte are EOF chars
     * */
    if((byte_read==0xFF)&&(uart2_last_byte==0xFF))
    {
      flag_end_of_frame_detected = true;
      break;
    }

    // rxbuf2[uart2_byte_index++] = ','; // Keep adding comma's only if needed

    // Update last byte, with anticaption of being read later
    uart2_last_byte = byte_read; //update with this recent byte

    rx_fifo_len--;
  }
  urxlen2 = uart2_byte_index;

  /**
   * if EOF is detected, commit data into nosplit buffer
   * */
  if(flag_end_of_frame_detected)
  {
    // Only 4 bytes
    simple_uart2_receive_frame_for_calibration[0] = rxbuf2[0];
    simple_uart2_receive_frame_for_calibration[1] = rxbuf2[1];
    simple_uart2_receive_frame_for_calibration[2] = rxbuf2[2];
    simple_uart2_receive_frame_for_calibration[3] = rxbuf2[3];
    flag_simple_uart2_receive_frame_for_calibration_updated = true;
    /**
     * Reset byte index for next frame
     * */
    uart2_byte_index = 0;
  }
  
  // after reading bytes from buffer clear UART interrupt status
  uart_clear_intr_status(UART_NUM_2, 
    UART_RXFIFO_TOUT_INT_CLR |
    //clearing more, maybe its the cause of crashes?
    UART_BRK_DET_INT_CLR |
    //clearing more, maybe its the cause of crashes?
    UART_CTS_CHG_INT_CLR |
    //clearing more, maybe its the cause of crashes?
    UART_DSR_CHG_INT_CLR |
    //clearing more, maybe its the cause of crashes?
    UART_RXFIFO_OVF_INT_CLR |
    UART_FRM_ERR_INT_CLR | // UART Frame Error
    //clearing more, maybe its the cause of crashes?
    UART_PARITY_ERR_INT_CLR |
    UART_TXFIFO_EMPTY_INT_CLR | // Needed to enable transmitting on serial
    UART_RXFIFO_FULL_INT_CLR 
  );


  #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
  gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
  #endif
  // gpio_set_level(GPIO_NUM_21, 1);

//  DEBUG_PIN1_SET(HIGH);
}

#endif

/**
 * @brief Define UART interrupt subroutine to ackowledge interrupt
 * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
 * 
 * Arriving data will be read (appended) to a global small buffer, each byte in will be checked. 
 * If special EOF chars are found, then the data in the global buffer is commited to the ringbuffer nosplit, which allows retreiving one item at a time
 * 
 * 
 **/
void IRAM_ATTR UART2_ISR_Static_NoSplitRingBuffer(void *arg)
{ 
//  DEBUG_PIN1_SET(LOW);
  
  uint16_t rx_fifo_len, status;
  // uint16_t i = 0;
  char byte_read = 0;
  bool flag_end_of_frame_detected = false;

  status = UART2.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer

  while(rx_fifo_len)
  {

    byte_read = UART2.fifo.rw_byte; // You can not directly access the UART0.fifo.rw_byte on esp32s2 but have to use READ_PERI_REG(UART_FIFO_AHB_REG(0))

    rxbuf2[uart2_byte_index++] = byte_read;
    if(uart2_byte_index>=UART2_ISR_TMP_BUFFER_SIZE-1)
    {
      uart2_byte_index = 0; // forced reset
      // We should never reach here, unless no EOF are detected then wrapping around will dump/lose all data
      break;
    }

    /**
     * Check if this byte and last byte are EOF chars
     * */
    if((byte_read==0xFF)&&(uart2_last_byte==0xFF))
    {
      flag_end_of_frame_detected = true;
      break;
    }

    // Update last byte, with anticaption of being read later
    uart2_last_byte = byte_read; //update with this recent byte

    rx_fifo_len--;
  }
  urxlen2 = uart2_byte_index;

  /**
   * if EOF is detected, commit data into nosplit buffer
   * */
  if(flag_end_of_frame_detected)
  {
    #ifdef ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER


      /**
       * New method, must write to a few circler buffers that is meant for sdcard
       * */
      // switch(uart_rss_tmp_buf_writer_index)
      // {
      //   case 0:
      //     uart_rss_tmp_buflen_0 = urxlen2;
      //     memcpy(uart_rss_tmp_buffer_0, rxbuf2, urxlen2);
      //     uart_rss_tmp_buf_writer_index = 1;
      //   break;
      //   case 1:
      //     uart_rss_tmp_buflen_1 = urxlen2;
      //     memcpy(uart_rss_tmp_buffer_1, rxbuf2, urxlen2);
      //     uart_rss_tmp_buf_writer_index = 0;
      //   break;
      // }


    #else
    BaseType_t dummyval;
    UBaseType_t res =  xRingbufferSendFromISR(pCONT_uart->settings.uart2.ringbuffer_handle, rxbuf2, urxlen2, &dummyval); // instead of ringbuffer, this probably also needs to be cycling buffers
    #endif // ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER

    /**
     * Reset byte index for next frame
     * */
    uart2_byte_index = 0;
  }
  
  // after reading bytes from buffer clear UART interrupt status
  uart_clear_intr_status(UART_NUM_2, 
    UART_RXFIFO_TOUT_INT_CLR |
    // UART_BRK_DET_INT_CLR |
    // UART_CTS_CHG_INT_CLR |
    // UART_DSR_CHG_INT_CLR |
    // UART_RXFIFO_OVF_INT_CLR |
    UART_FRM_ERR_INT_CLR | // UART Frame Error
    // UART_PARITY_ERR_INT_CLR |
    UART_TXFIFO_EMPTY_INT_CLR | // Needed to enable transmitting on serial
    UART_RXFIFO_FULL_INT_CLR 
  );


  #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
  gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
  #endif

//  DEBUG_PIN1_SET(HIGH);
}


#endif // ENABLE_HARDWARE_UART_2

#endif
