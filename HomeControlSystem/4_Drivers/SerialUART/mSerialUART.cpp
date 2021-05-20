#include "mSerialUART.h"

/**
 * All Serial buffers will store in ringbuffers, then set a flag that it is ready. Other modules will then "pop" off some of the 
 * data from the buffers and free the memory.
 * */

#ifdef USE_MODULE_DRIVERS_SERIAL_UART

const char* mSerialUART::PM_MODULE_DRIVERS_SERIAL_UART_CTR = D_MODULE_DRIVERS_SERIAL_UART_CTR;
const char* mSerialUART::PM_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR = D_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR;


int8_t mSerialUART::Tasker(uint8_t function, JsonParserObject obj){

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

   break;
   case FUNC_EVERY_SECOND:{

    // AddLog(LOG_LEVEL_TEST, PSTR("[%d] \"%s\""),pCONT_uart->urxlen2,(char*)pCONT_uart->rxbuf2);

    // Serial.println(pCONT_uart->urxlen2);
    // for(int i=0;i<10;i++){   Serial.print((char)pCONT_uart->rxbuf2[i]); }
    // Serial.println();
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
      uint16_t bytes_in_line = GetReceiveBuffer1(buffer, sizeof(buffer), '\n', false);

      if(buffer[0]){
        AddLog(LOG_LEVEL_TEST, PSTR("UART1 >> [%d] \"%s\""), bytes_in_line, buffer);
      }
      
      char buffer2[100] = {0};
      uint16_t bytes_in_line2 = GetReceiveBuffer2(buffer2, sizeof(buffer2), '\n', false);

      if(buffer2[0]){
        AddLog(LOG_LEVEL_TEST, PSTR("UART2 >> [%d] \"%s\""), bytes_in_line2, buffer2);
      }
      

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

  #ifdef ENABLE_HARDWARE_UART_0
  if(pCONT_pins->PinUsed(GPIO_HWSERIAL0_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL0_RING_BUFFER_RX_ID)) {
    settings.uart0.receive_interrupts_enable = true;
    settings.uart0.baud = 115200;
    settings.uart0.gpio.tx = pCONT_pins->GetPin(GPIO_HWSERIAL0_RING_BUFFER_TX_ID);
    settings.uart0.gpio.rx = pCONT_pins->GetPin(GPIO_HWSERIAL0_RING_BUFFER_RX_ID);
    AddLog(LOG_LEVEL_INFO, PSTR("UART0 RingBuffer Interrupts pins: TX[%d] RX[%d]"),settings.uart0.gpio.tx, settings.uart0.gpio.rx);
  }else{
    settings.uart0.receive_interrupts_enable = false;
  }
  #endif // ENABLE_HARDWARE_UART_0
  
  #ifdef ENABLE_HARDWARE_UART_1
  if(pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_RX_ID)) {
    settings.uart1.receive_interrupts_enable = true;
    settings.uart1.baud = HARDWARE_UART_1_BAUD_RATE_SPEED;
    settings.uart1.gpio.tx = pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID);
    settings.uart1.gpio.rx = pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID);
    AddLog(LOG_LEVEL_INFO, PSTR("UART1 RingBuffer Interrupts pins: TX[%d] RX[%d]"),settings.uart1.gpio.tx, settings.uart1.gpio.rx);
  }else{
    settings.uart1.receive_interrupts_enable = false;
  }
  #endif // ENABLE_HARDWARE_UART_1

  #ifdef ENABLE_HARDWARE_UART_2
  if(pCONT_pins->PinUsed(GPIO_HWSERIAL2_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL2_RING_BUFFER_RX_ID)) {
    settings.uart2.receive_interrupts_enable = true;
    settings.uart2.baud = HARDWARE_UART_2_BAUD_RATE_SPEED;
    settings.uart2.gpio.tx = pCONT_pins->GetPin(GPIO_HWSERIAL2_RING_BUFFER_TX_ID);
    settings.uart2.gpio.rx = pCONT_pins->GetPin(GPIO_HWSERIAL2_RING_BUFFER_RX_ID);
    AddLog(LOG_LEVEL_INFO, PSTR("UART2 RingBuffer Interrupts pins: TX[%d] RX[%d]"),settings.uart2.gpio.tx, settings.uart2.gpio.rx);
  }else{
    settings.uart2.receive_interrupts_enable = false;
  }
  #endif // ENABLE_HARDWARE_UART_2

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
  
}



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
uint16_t mSerialUART::GetReceiveBuffer1(char* output_buf, uint16_t output_len, char optional_read_until_char, bool flag_clear_buffer_after_read)
{
  uint16_t bytes_to_read =0;
// xRingbufferPrintInfo(settings.uart2.ringbuffer_handle);
      // //Receive an item from no-split ring buffer
      size_t item_size;
      // Wait at most 1ms to read from buffer
      char*  item = (char *)xRingbufferReceive(pCONT_uart->settings.uart1.ringbuffer_handle, &item_size, pdMS_TO_TICKS(100));

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


// if(flag_clear_buffer_after_read)
// {
        //Return Item
        vRingbufferReturnItem(pCONT_uart->settings.uart1.ringbuffer_handle, (void *)item); // Free memory
        // AddLog(LOG_LEVEL_TEST, PSTR("sizeA=%d"),xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
// }
      } 
      // else {
      //   //Failed to receive item EMPTY
      //   // AddLog(LOG_LEVEL_TEST, PSTR("Failed to receive item"));
      // }

  return bytes_to_read;

}

/**
 * Returns: Bytes read from buffer and thus freed
 * Arg:     Buffer to write into and its size, or simply, how much to read at a time
 * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
 * For me, use new line
 * */
uint16_t mSerialUART::GetReceiveBuffer2(char* output_buf, uint16_t output_len, char optional_read_until_char, bool flag_clear_buffer_after_read)
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


// if(flag_clear_buffer_after_read)
// {
        //Return Item
        vRingbufferReturnItem(pCONT_uart->settings.uart2.ringbuffer_handle, (void *)item); // Free memory
        // AddLog(LOG_LEVEL_TEST, PSTR("sizeA=%d"),xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
// }
      } 
      // else {
      //   //Failed to receive item EMPTY
      //   // AddLog(LOG_LEVEL_TEST, PSTR("Failed to receive item"));
      // }

  return bytes_to_read;

}
/**
 * Returns: Bytes read from buffer and thus freed
 * Arg:     Buffer to write into and its size, or simply, how much to read at a time
 * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
 * For me, use new line
 * */
uint16_t mSerialUART::GetReceiveBufferPartial2(char* output_buf, uint16_t output_len, char optional_read_until_char, bool flag_clear_buffer_after_read)
{
  uint16_t bytes_to_read =0;
// xRingbufferPrintInfo(settings.uart2.ringbuffer_handle);
      // //Receive an item from no-split ring buffer
      size_t item_size;
      // Wait at most 1ms to read from buffer
      char*  item = (char *)xRingbufferReceiveUpTo(pCONT_uart->settings.uart2.ringbuffer_handle, &item_size, pdMS_TO_TICKS(100), output_len);

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


// if(flag_clear_buffer_after_read)
// {
        //Return Item
        vRingbufferReturnItem(pCONT_uart->settings.uart2.ringbuffer_handle, (void *)item); // Free memory
        // AddLog(LOG_LEVEL_TEST, PSTR("sizeA=%d"),xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
// }
      } 
      // else {
      //   //Failed to receive item EMPTY
      //   // AddLog(LOG_LEVEL_TEST, PSTR("Failed to receive item"));
      // }

  return bytes_to_read;

}





#endif
