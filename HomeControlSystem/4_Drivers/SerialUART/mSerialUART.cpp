#include "mSerialUART.h"

/**
 * All Serial buffers will store in ringbuffers, then set a flag that it is ready. Other modules will then "pop" off some of the 
 * data from the buffers and free the memory.
 * */

#ifdef USE_MODULE_DRIVERS_SERIAL_UART

const char* mSerialUART::PM_MODULE_DRIVERS_SERIAL_UART_CTR = D_MODULE_DRIVERS_SERIAL_UART_CTR;
const char* mSerialUART::PM_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR = D_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR;

// static uart_isr_handle_t *uart2_handle_console;
/*********************************************************************************************************************************************************************************************************************************************************************************************************
 ******* INIT **************************************************************************************************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************************************************************************************************* ***************/


int8_t mSerialUART::Tasker(uint8_t function, JsonParserObject obj){


// Serial.println();

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    Pre_Init_Pins();
  }else
  if(function == FUNC_INIT){
    Init();
  }

  // Only continue in to tasker if module was configured properly
  //if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
   case FUNC_LOOP:

    // if(pCONT_uart->urxlen2)
    // {
    // Serial.println(pCONT_uart->urxlen2);
    // for(int i=0;i<10;i++){   Serial.print(pCONT_uart->rxbuf2[i]); }
    // Serial.println();
    // pCONT_uart->urxlen2 = 0; // reset
    // }


   break;
  case FUNC_EVERY_SECOND:{

    AddLog(LOG_LEVEL_TEST, PSTR("[%d] \"%s\""),pCONT_uart->urxlen2,pCONT_uart->rxbuf2);

    // Serial.println(pCONT_uart->urxlen2);
    // for(int i=0;i<10;i++){   Serial.print((char)pCONT_uart->rxbuf2[i]); }
    // Serial.println();
    ESP_LOGI("test", "Initializing SD card");
      // AddLog(LOG_LEVEL_INFO, PSTR("buffU2=%d"),xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
  
      // xRingbufferPrintInfo(settings.uart2.ringbuffer_handle);
      // //Receive an item from no-split ring buffer
      // size_t item_size;
      // char*  item = (char *)xRingbufferReceive(pCONT_uart->settings.uart2.ringbuffer_handle, &item_size, pdMS_TO_TICKS(1000));

      // //Check received item
      // if (item != NULL) {
      //   Serial.printf("UART2:>%d %d sizeB=%d\n\r",item_size,millis(),xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
      //   //Print item
      //   for(int i = 0; i < item_size; i++) {
      //     Serial.printf("%c", item[i]);
      //   }
      //   //Return Item
      //   vRingbufferReturnItem(pCONT_uart->settings.uart2.ringbuffer_handle, (void *)item); // Free memory
      //   Serial.printf("\n\r sizeA=%d\n\r",xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
      // } else {
      //   //Failed to receive item
      //   Serial.printf("Failed to receive item\n");
      // }

      char buffer[100] = {0};
      uint16_t bytes_in_line = GetReceiveBuffer2(buffer, sizeof(buffer), '\n');

      if(buffer){
        AddLog(LOG_LEVEL_TEST, PSTR("size=%d, \"%s\""), bytes_in_line, buffer);
      }else{
        AddLog(LOG_LEVEL_TEST, PSTR("size=%d, \"none\""), bytes_in_line);
      }
      
    ESP_LOGI("test", "FUNC_EVERY_SECOND");

    }
    break;
    case FUNC_EVERY_MINUTE: {

      // xRingbufferPrintInfo(settings.uart2.ringbuffer_handle);
      // //Receive an item from no-split ring buffer
      // size_t item_size;
      // char*  item = (char *)xRingbufferReceive(pCONT_uart->settings.uart2.ringbuffer_handle, &item_size, pdMS_TO_TICKS(1000));

      // //Check received item
      // if (item != NULL) {
      //   Serial.printf("UART2:>%d %d sizeB=%d\n\r",item_size,millis(),xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
      //   //Print item
      //   for(int i = 0; i < item_size; i++) {
      //     Serial.printf("%c", item[i]);
      //   }
      //   //Return Item
      //   vRingbufferReturnItem(pCONT_uart->settings.uart2.ringbuffer_handle, (void *)item); // Free memory
      //   Serial.printf("\n\r sizeA=%d\n\r",xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
      // } else {
      //   //Failed to receive item
      //   Serial.printf("Failed to receive item\n");
      // }

    }
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END Tasker


void mSerialUART::Pre_Init_Pins()
{

  if(pCONT_pins->PinUsed(GPIO_HWSERIAL0_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL0_RING_BUFFER_RX_ID)) {
    settings.uart0.receive_interrupts_enable = true;
    settings.uart0.baud = 115200;
    settings.uart0.gpio.tx = pCONT_pins->GetPin(GPIO_HWSERIAL0_RING_BUFFER_TX_ID);
    settings.uart0.gpio.rx = pCONT_pins->GetPin(GPIO_HWSERIAL0_RING_BUFFER_RX_ID);
    AddLog(LOG_LEVEL_INFO, PSTR("UART0 RingBuffer Interrupts pins: TX[%d] RX[%d]"),settings.uart0.gpio.tx, settings.uart0.gpio.rx);
  }else{
    settings.uart0.receive_interrupts_enable = false;
  }
  
  if(pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_RX_ID)) {
    settings.uart1.receive_interrupts_enable = true;
    settings.uart1.baud = 115200;
    settings.uart1.gpio.tx = pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID);
    settings.uart1.gpio.rx = pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID);
    AddLog(LOG_LEVEL_INFO, PSTR("UART1 RingBuffer Interrupts pins: TX[%d] RX[%d]"),settings.uart1.gpio.tx, settings.uart1.gpio.rx);
  }else{
    settings.uart1.receive_interrupts_enable = false;
  }

  if(pCONT_pins->PinUsed(GPIO_HWSERIAL2_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL2_RING_BUFFER_RX_ID)) {
    settings.uart2.receive_interrupts_enable = true;
    settings.uart2.baud = 115200;
    settings.uart2.gpio.tx = pCONT_pins->GetPin(GPIO_HWSERIAL2_RING_BUFFER_TX_ID);
    settings.uart2.gpio.rx = pCONT_pins->GetPin(GPIO_HWSERIAL2_RING_BUFFER_RX_ID);
    AddLog(LOG_LEVEL_INFO, PSTR("UART2 RingBuffer Interrupts pins: TX[%d] RX[%d]"),settings.uart2.gpio.tx, settings.uart2.gpio.rx);
  }else{
    settings.uart2.receive_interrupts_enable = false;
  }

}


void mSerialUART::Init(void)
{

  #ifdef ENABLE_HARDWARE_UART_1
  if(settings.uart1.receive_interrupts_enable)
  {
    init_UART1_RingBuffer();
    init_UART1_ISR();
  }
  #endif // ENABLE_HARDWARE_UART_1

  #ifdef ENABLE_HARDWARE_UART_2
  if(settings.uart2.receive_interrupts_enable)
  {
    init_UART2_RingBuffer();
    init_UART2_ISR();
  }
  #endif // ENABLE_HARDWARE_UART_2

  
esp_log_level_set("test", ESP_LOG_VERBOSE);
  
}


/******************************************************************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************************************************************************
****** UART2 ************************************************************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************************************************************************/

#ifdef ENABLE_HARDWARE_UART_2
#ifdef ENABLE_UART_RINGBUFFERS
void mSerialUART::init_UART2_RingBuffer()
{

  settings.uart2.ringbuffer_handle = xRingbufferCreate(RINGBUFFER_HANDLE_2_LENGTH, RINGBUF_TYPE_BYTEBUF);
  if (settings.uart2.ringbuffer_handle != NULL) {
    settings.uart2.initialised = true;
  }

  char data_ctr[] = "UART2 RingBuf Init\0";
  UBaseType_t res =  xRingbufferSend(settings.uart2.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
  if (res != pdTRUE) {
    AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
    settings.uart2.initialised = false; //disable if false
  }

}

#endif // ENABLE_UART_RINGBUFFERS

/**
 * @brief init UART2 ISR routine
 * @note
 * @param void none
 * @return none
 */
void mSerialUART::init_UART2_ISR(){

  AddLog(LOG_LEVEL_DEBUG, PSTR("init_UART2_ISR Starting..."));

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
    uart_intr_handle_u2_static, // Interrupt handler function.
    NULL,                       // parameter for handler function
    ESP_INTR_FLAG_IRAM,         // Flags used to allocate the interrupt. One or multiple (ORred) ESP_INTR_FLAG_* values. See esp_intr_alloc.h for more info. 
    uart2_handle_console        // Pointer to return handle. If non-NULL, a handle for the interrupt will be returned here.
  );
  
  AddLog(LOG_LEVEL_DEBUG, PSTR(DEBUG_INSERT_PAGE_BREAK "init_UART2_ISR Started %d buffer size"),settings.uart2.ring_buffer_size_rx);

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

// // Receive buffer to collect incoming data
// uint8_t rxbuf[256];
// // Register to collect data length
// uint16_t urxlen;
/**
 * @brief Define UART interrupt subroutine to ackowledge interrupt
 * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
 **/
void IRAM_ATTR uart_intr_handle_u2_static(void *arg)
{ 
  
  uint16_t rx_fifo_len, status;
  uint16_t i = 0;

  status = UART2.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer

  // uint32_t timeout = millis();
  
  while(rx_fifo_len)
  {
    pCONT_uart->rxbuf2[i++] = UART2.fifo.rw_byte; // read all bytes
    // if(i>=RINGBUFFER_HANDLE_2_LENGTH-1)
    // {
    //   break;
    // }
    // if(abs(millis()-timeout)>50)
    // {
    //   break;
    // }
    rx_fifo_len--;
  }
  pCONT_uart->urxlen2 = i;

  // BaseType_t dummyval;
  // UBaseType_t res =  xRingbufferSendFromISR(pCONT_uart->settings.uart2.ringbuffer_handle, pCONT_uart->rxbuf2, pCONT_uart->urxlen2, &dummyval);
  
  // after reading bytes from buffer clear UART interrupt status
  uart_clear_intr_status(UART_NUM_2, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
  
  #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
  gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
  #endif


// uint16_t rx_fifo_len, status;
//   uint16_t i;
  
//   status = UART0.int_st.val; // read UART interrupt Status
//   rx_fifo_len = UART0.status.rxfifo_cnt; // read number of bytes in UART buffer
  
//   while(rx_fifo_len){
//    rxbuf[i++] = UART0.fifo.rw_byte; // read all bytes
//    rx_fifo_len--;
//  }
  
//  // after reading bytes from buffer clear UART interrupt status
//  uart_clear_intr_status(UART_NUM_2, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);



  // uart_write_bytes(UART_NUM_0, (const char*) "RX Done", 7);

}

#endif // ENABLE_HARDWARE_UART_2


/******************************************************************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************************************************************************
****** Shared ************************************************************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************************************************************************/

/**
 * @brief Function to get data from ringbuffer, with optional flag to keep the data read on the buffer (ie dont give that space back, mostly as debugging tool)
 * */


/**
 * Returns: Bytes read from buffer and thus freed
 * Arg:     Buffer to write into and its size, or simply, how much to read at a time
 * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
 * For me, use new line
 * */
uint16_t mSerialUART::GetReceiveBuffer2(char* output_buf, uint16_t output_len, char optional_read_until_char)
{
  uint16_t bytes_to_read =0;
// xRingbufferPrintInfo(settings.uart2.ringbuffer_handle);
      // //Receive an item from no-split ring buffer
      size_t item_size;
      // Wait at most 1ms to read from buffer
      char*  item = (char *)xRingbufferReceive(pCONT_uart->settings.uart2.ringbuffer_handle, &item_size, pdMS_TO_TICKS(100));

      //Check received item
      if (item != NULL) {
        // AddLog(LOG_LEVEL_TEST, PSTR("UART2:>%d %d sizeB=%d"),item_size,millis(),xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
        //Print item
        // for(int i = 0; i < item_size; i++) {
        //   Serial.printf("%c", item[i]);          
        // }

        bytes_to_read = item_size > output_len ? output_len : item_size;

        // AddLog(LOG_LEVEL_TEST, PSTR("bytes_to_read=%d, item_size=%d, output_len=%d"),bytes_to_read,item_size,output_len);
        // Read from buffer
        // snprintf(output_buf, output_len)
        memcpy(output_buf,item,bytes_to_read*sizeof(char));

        AddLog(LOG_LEVEL_TEST, PSTR("output_buf=%s"),output_buf);


        //Return Item
        vRingbufferReturnItem(pCONT_uart->settings.uart2.ringbuffer_handle, (void *)item); // Free memory
        // AddLog(LOG_LEVEL_TEST, PSTR("sizeA=%d"),xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
      } 
      // else {
      //   //Failed to receive item EMPTY
      //   // AddLog(LOG_LEVEL_TEST, PSTR("Failed to receive item"));
      // }

  return bytes_to_read;

}




uint8_t mSerialUART::ConstructJSON_Settings(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();  

    JsonBuilderI->Add_P(PM_JSON_TIME, 1000);
    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}
uint8_t mSerialUART::ConstructJSON_UARTInfo(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();  

    JsonBuilderI->Add_P(PM_JSON_TIME, 1000);

    JBI->Level_Start("UART2");
      JBI->Add("receive_interrupts_enable", settings.uart2.receive_interrupts_enable);
    JBI->Level_End();

    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}


////////////////////// START OF MQTT /////////////////////////

#ifdef USE_MODULE_NETWORK_MQTT

/**
 * @brief Init the mqtthandlers
 * */
void mSerialUART::MQTTHandler_Init(){

  struct handler<mSerialUART>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_SETTINGS_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSerialUART::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_uartinfo_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_UARTINFO_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_UARTINFO_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSerialUART::ConstructJSON_UARTInfo;

} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSerialUART::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSerialUART::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mSerialUART::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_DRIVERS_SERIAL_UART_ID, handle, id);
  }
}
#endif // USE_MODULE_NETWORK_MQTT




#endif
