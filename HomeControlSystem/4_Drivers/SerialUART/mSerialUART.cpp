#include "mSerialUART.h"

/**
 * All Serial buffers will store in ringbuffers, then set a flag that it is ready. Other modules will then "pop" off some of the 
 * data from the buffers and free the memory.
 * */

#ifdef USE_MODULE_DRIVERS_SERIAL_UART

const char* mSerialUART::PM_MODULE_DRIVERS_SERIAL_UART_CTR = D_MODULE_DRIVERS_SERIAL_UART_CTR;
const char* mSerialUART::PM_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR = D_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR;

  uint8_t simple_uart2_receive_frame_for_calibration[4] = {0};
  bool flag_simple_uart2_receive_frame_for_calibration_updated = false;

int8_t mSerialUART::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT: 
      Pre_Init_Pins(); 
    break;
    case FUNC_INIT: 
      Init(); 
    break;
    /**
     * Special case, I want to loop and init this when another modules says
     * */
    case FUNC_LOOP:

      #ifndef USE_MODULE_DRIVERS_GPS
        // Finsihed with manual control, start ISRs
        pCONT_uart->flag_init_buffers_and_start_isrs = true;
      #endif
    
     if(flag_init_buffers_and_start_isrs && !settings.fEnableModule)
     {       
      StartISR_RingBuffers(); 
     }
     
    #ifndef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

This might be causing delays when not needed in measurements
     
//temp dump data so it doesnt fill

      size_t rss_data_read_size = 0;
      char *rss_data_read = (char *)xRingbufferReceive(pCONT_uart->settings.uart2.ringbuffer_handle, &rss_data_read_size, pdMS_TO_TICKS(1000));
      if (rss_data_read != NULL) { // Read from buffer
        //memcpy(isr_rss_buffer,rss_data_read,rss_data_read_size);
        // for(int i=0;i<rss_data_read_size;i++){
        //   BufferWriterI->Append_P(PSTR("%d%s"), rss_data_read[i], i<rss_data_read_size-1? ",": ""); 
        // }
        vRingbufferReturnItem(pCONT_uart->settings.uart2.ringbuffer_handle, (void *)rss_data_read); // Free memory
      }
      #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

    // case FUNC_UPTIME_10_SECONDS: 
    // case FUNC_UPTIME_30_SECONDS: 
    //   StartISR_RingBuffers(); 
    break;
  }

  // Only continue in to tasker if module was configured properly
  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }


// static const char *TX_TASK_TAG = "TX_TASK";
// sendData(TX_TASK_TAG, TX_TASK_TAG);


  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
   case FUNC_LOOP:{

      // BufferWriterI->Clear();
      // uint16_t bytes_in_line = pCONT_uart->GetRingBufferDataAndClear(2, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
      // if(BufferWriterI->GetLength()){
      //   AddLog(LOG_LEVEL_TEST, PSTR("GPS UART%d >> [%d] \"%s\""), 2, bytes_in_line, BufferWriterI->GetPtr());
      // }


    // #ifdef ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
    // // for(
    //   int i=2;
    // //   i<3;i++)
    // // {
    //   BufferWriterI->Clear();
    //   uint16_t bytes_in_line = GetRingBufferDataAndClear(i, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
    //   if(bytes_in_line){
    //     AddLog(LOG_LEVEL_TEST, PSTR("UART%d >> [%d]"), i, bytes_in_line);
    //     // AddLog(LOG_LEVEL_TEST, PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
    //   }
    // // }
    // #endif

      // char buffer2[500] = {0};
      // uint16_t bytes_in_line2 = GetRingBufferDataAndClear(2, buffer2, sizeof(buffer2), '\n', false);

      // if(buffer2[0]){
      //   AddLog(LOG_LEVEL_TEST, PSTR("UART2 >> [%d] \"%s\""), bytes_in_line2, buffer2);
      // }
   }
   break;
   case FUNC_EVERY_SECOND:{


// // for(
//       int i=2;
//     //   i<3;i++)
//     // {
//       BufferWriterI->Clear();
//       uint16_t bytes_in_line = GetRingBufferDataAndClear(i, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
//       if(bytes_in_line){//strlen(BufferWriterI->GetPtr())){
//         AddLog(LOG_LEVEL_TEST, PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
//       }else{
//         AddLog(LOG_LEVEL_TEST, PSTR("UART%d >> EMPTY"));//, i, bytes_in_line, BufferWriterI->GetPtr());

//       }
//     // }
//     // #endif


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

      #ifdef ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER


      #endif // ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER


    }
    break;
    case FUNC_EVERY_MINUTE: {


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
    // init_UART1_pins();
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
    // init_UART1_pins();
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
    // init_UART2_pins();
    AddLog(LOG_LEVEL_INFO, PSTR("UART2 RingBuffer Interrupts pins: TX[%d] RX[%d]"),settings.uart2.gpio.tx, settings.uart2.gpio.rx);
  }else{
    settings.uart2.receive_interrupts_enable = false;
  }
  #endif // ENABLE_HARDWARE_UART_2

}


void mSerialUART::Init(void)
{

  // #ifdef ENABLE_HARDWARE_UART_1
  // if(settings.uart1.receive_interrupts_enable)
  // {
  //   init_UART1_RingBuffer();
  //   init_UART1_ISR();
  // }
  // #endif // ENABLE_HARDWARE_UART_1

  // #ifdef ENABLE_HARDWARE_UART_2
  // if(settings.uart2.receive_interrupts_enable)
  // {
  //   init_UART2_RingBuffer();
  //   init_UART2_ISR();
  // }
  // #endif // ENABLE_HARDWARE_UART_2
  
}


void mSerialUART::StartISR_RingBuffers()
{

  AddLog(LOG_LEVEL_TEST, PSTR("delayed start of uart interrupts for methods which need basic serial.read"));

  #ifdef ENABLE_HARDWARE_UART_1
  if(settings.uart1.receive_interrupts_enable)
  {
    init_UART1_pins();
    init_UART1_RingBuffer();
    init_UART1_ISR();
    settings.uart1.initialised = true;
    settings.fEnableModule = true;
  }
  #endif // ENABLE_HARDWARE_UART_1

DEBUG_LINE_HERE;
  #ifdef ENABLE_HARDWARE_UART_2
  if(settings.uart2.receive_interrupts_enable)
  {
    // settings.uart2.ring_buffer_size_rx = 
    init_UART2_pins();
    init_UART2_RingBuffer();
    init_UART2_ISR();
    settings.uart2.initialised = true;
    settings.fEnableModule = true;
  }
  #endif // ENABLE_HARDWARE_UART_2


DEBUG_LINE_HERE;

}

int mSerialUART::sendData(const char* logName, const char* data)
{
  const int len = strlen(data);
  const int txBytes = uart_write_bytes(UART_NUM_2, data, len);
  ESP_LOGI(logName, "Wrote %d bytes", txBytes);
  return txBytes;
}


/******************************************************************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************************************************************************
****** Shared ************************************************************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************************************************************************/

/**
 * Universal to all UARTs, get RingBuffer
 * Returns: Bytes read from buffer and thus freed
 * Arg:     Buffer to write into and its size, or simply, how much to read at a time
 * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
 * @return item_size number of bytes read from buffer
 * */
uint16_t mSerialUART::GetRingBufferDataAndClear(uint8_t uart_num, char* buffer, uint16_t buflen, char optional_read_until_char, bool flag_clear_buffer_after_read)
{

  // RingbufHandle_t handle_tmp;
  // ringbuf_type_t ringbuffer_format_type_tmp;
  // switch(uart_num)
  // {
  //   default:
  //   case 0: 
  //     if(!settings.uart0.initialised)
  //     {
  //       return 0;
  //     }
  //     handle_tmp = settings.uart0.ringbuffer_handle;
  //   break;
  //   case 1:
  //     if(!settings.uart1.initialised)
  //     {
  //       return 0;
  //     } 
  //     handle_tmp = settings.uart1.ringbuffer_handle;
  //   break;
  //   case 2: 
  //     if(!settings.uart2.initialised)
  //     {
  //       return 0;
  //     }
  //     handle_tmp = settings.uart2.ringbuffer_handle;
  //   break;
  // }

  // //delayed start check
  // // if(handle_tmp == nullptr)
  // // {
  // //   return 0; // not ready to continue
  // // }
  
  // // Receive an item from no-split ring buffer
  // size_t item_size = 0;
  // // Wait at most 1ms to read from buffer, read a maximum number of bytes
  // char*  item = (char *)xRingbufferReceiveUpTo(handle_tmp, &item_size, pdMS_TO_TICKS(100), buflen);


  RingbufHandle_t handle_tmp;
  ringbuf_type_t ringbuffer_format_type_tmp;
  switch(uart_num)
  {
    default:
  #ifdef ENABLE_HARDWARE_UART_0
    case 0: 
      if(!settings.uart0.initialised)
      {
        return 0;
      }
      handle_tmp = settings.uart0.ringbuffer_handle;
      ringbuffer_format_type_tmp = settings.uart0.ringbuffer_format_type;
    break;
  #endif // ENABLE_HARDWARE_UART_0
  #ifdef ENABLE_HARDWARE_UART_1
    case 1:
      if(!settings.uart1.initialised)
      {
        return 0;
      } 
      handle_tmp = settings.uart1.ringbuffer_handle;
      ringbuffer_format_type_tmp = settings.uart1.ringbuffer_format_type;
    break;
  #endif // ENABLE_HARDWARE_UART_1
  #ifdef ENABLE_HARDWARE_UART_2
    case 2: 
      if(!settings.uart2.initialised)
      {
        return 0;
      }
      handle_tmp = settings.uart2.ringbuffer_handle;
      ringbuffer_format_type_tmp = settings.uart2.ringbuffer_format_type;
    break;
  #endif // ENABLE_HARDWARE_UART_2
  }

  //delayed start check
  // if(handle_tmp == nullptr)
  // {
  //   return 0; // not ready to continue
  // }
  
  // Receive an item from no-split ring buffer
  size_t item_size = 0;
  // Wait at most 1ms to read from buffer, read a maximum number of bytes

char *item = nullptr;

switch(ringbuffer_format_type_tmp)
{
  case RINGBUF_TYPE_BYTEBUF:
    item = (char *)xRingbufferReceiveUpTo(handle_tmp, &item_size, pdMS_TO_TICKS(100), buflen);

  break;
  case RINGBUF_TYPE_NOSPLIT:
    item = (char *)xRingbufferReceive(handle_tmp, &item_size, pdMS_TO_TICKS(1000));


  break;


}


  //Check received item
  if (item != NULL) {
    // Read from buffer
    memcpy(buffer,item,item_size);

    //AddLog(LOG_LEVEL_TEST, PSTR("xRingbufferReceiveUpTo=\"%s\""),buffer);

    // if(flag_clear_buffer_after_read)
    // {
      //Return Item
      vRingbufferReturnItem(handle_tmp, (void *)item); // Free memory
    // }
  } 

  return item_size;

}

// /**
//  * Universal to all UARTs, get RingBuffer
//  * Returns: Bytes read from buffer and thus freed
//  * Arg:     Buffer to write into and its size, or simply, how much to read at a time
//  * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
//  * @return item_size number of bytes read from buffer
//  * */
// uint16_t mSerialUART::GetRingBufferDataAndClearUntilSpecialDelimeter(uint8_t uart_num, char* buffer, uint16_t buflen)
// {

//   RingbufHandle_t handle_tmp;
//   switch(uart_num)
//   {
//     default:
//     case 0: 
//       if(!settings.uart0.initialised)
//       {
//         return 0;
//       }
//       handle_tmp = settings.uart0.ringbuffer_handle;
//     break;
//     case 1:
//       if(!settings.uart1.initialised)
//       {
//         return 0;
//       } 
//       handle_tmp = settings.uart1.ringbuffer_handle;
//     break;
//     case 2: 
//       if(!settings.uart2.initialised)
//       {
//         return 0;
//       }
//       handle_tmp = settings.uart2.ringbuffer_handle;
//     break;
//   }

// //   //delayed start check
// //   // if(handle_tmp == nullptr)
// //   // {
// //   //   return 0; // not ready to continue
// //   // }
// //   uint16_t index_of_end_of_frame = 0;
// //   bool flag_found_eof = false;

// //   /**
// //    * First read, with amount greater than expected until next frame 0xFF 0xFF
// //    * */
// //   // Receive an item from no-split ring buffer
// //   size_t item1_size = 0;
// //   // Wait at most 1ms to read from buffer, read a maximum number of bytes
// //   char*  item1 = (char *)xRingbufferReceiveUpTo(handle_tmp, &item1_size, pdMS_TO_TICKS(100), buflen);



// //   /**
// //    * Read, then test this memory for the special char, and record the index (ie max length I need to read on next receiveupto)
// //    * */
// //   //Check received item
// //   if (item1 != NULL) {
// //     // Read from buffer
// //     memcpy(buffer,item1,item1_size);


// //   for(int i=0;i<item1_size;i++)
// //   {
// //     Serial.print(buffer[i]);
// //     Serial.print('-');
// //   }
// //     Serial.println();
// //     Serial.println();


// //     for(int i=0;i<item1_size;i++)
// //     {
// //       //first byte
// //       if(buffer[i] = 0xFF)
// //       {
// //         if(buffer[i+1] = 0xFF)
// //         {
// //           index_of_end_of_frame = i+1;
// //           break;
// //         }
// //       }
// //     }
// //   }

// //   /**
// //    *  Read again, but only until the EOF ends
// //    * */
// //   if(flag_found_eof)
// //   {
// //     buflen = index_of_end_of_frame;
// //   }

// // */

  
//   // Receive an item from no-split ring buffer
//   size_t item_size = 0;



//   // Wait at most 1ms to read from buffer, read a maximum number of bytes
//   char*  item = nullptr;
  
  
//   item = (char *)xRingbufferReceiveUpTo(handle_tmp, &item_size, pdMS_TO_TICKS(100), buflen);

//   //Check received item
//   if (item != NULL) {
//     // Read from buffer
//     memcpy(buffer,item,item_size);

//     AddLog(LOG_LEVEL_TEST, PSTR("xRingbufferReceiveUpTo=\"%s\""),buffer);

//     // if(flag_clear_buffer_after_read)
//     // {
//       //Return Item
//       vRingbufferReturnItem(handle_tmp, (void *)item); // Free memory
//     // }
//   } 

//   return item_size;

// }



/**
 * Universal to all UARTs, get RingBuffer
 * Returns: Bytes read from buffer and thus freed
 * Arg:     Buffer to write into and its size, or simply, how much to read at a time
 * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
 * @return item_size number of bytes read from buffer
 * */
uint16_t mSerialUART::GetSingleItemFromNoSplitRingBuffer(uint8_t uart_num, char* buffer, uint16_t buflen)
{

  RingbufHandle_t handle_tmp;
  ringbuf_type_t ringbuffer_format_type_tmp;
  switch(uart_num)
  {
    default:
    case 0: 
  #ifdef ENABLE_HARDWARE_UART_0
      if(!settings.uart0.initialised)
      {
        return 0;
      }
      handle_tmp = settings.uart0.ringbuffer_handle;
      ringbuffer_format_type_tmp = settings.uart0.ringbuffer_format_type;
    break;
  #endif // ENABLE_HARDWARE_UART_0
  #ifdef ENABLE_HARDWARE_UART_1
    case 1:
      if(!settings.uart1.initialised)
      {
        return 0;
      } 
      handle_tmp = settings.uart1.ringbuffer_handle;
      ringbuffer_format_type_tmp = settings.uart1.ringbuffer_format_type;
    break;
  #endif // ENABLE_HARDWARE_UART_1
  #ifdef ENABLE_HARDWARE_UART_2
    case 2: 
      if(!settings.uart2.initialised)
      {
        return 0;
      }
      handle_tmp = settings.uart2.ringbuffer_handle;
      ringbuffer_format_type_tmp = settings.uart2.ringbuffer_format_type;
    break;
  #endif // ENABLE_HARDWARE_UART_2
  }

  //delayed start check
  // if(handle_tmp == nullptr)
  // {
  //   return 0; // not ready to continue
  // }
  
  // Receive an item from no-split ring buffer
  size_t item_size = 0;
  // Wait at most 1ms to read from buffer, read a maximum number of bytes

char *item = nullptr;

switch(ringbuffer_format_type_tmp)
{
  case RINGBUF_TYPE_BYTEBUF:
    item = (char *)xRingbufferReceiveUpTo(handle_tmp, &item_size, pdMS_TO_TICKS(100), buflen);

  break;
  case RINGBUF_TYPE_NOSPLIT:
    item = (char *)xRingbufferReceive(handle_tmp, &item_size, pdMS_TO_TICKS(1000));


  break;


}

  // char*  item = (char *)xRingbufferReceiveUpTo(handle_tmp, &item_size, pdMS_TO_TICKS(100), buflen);
  // char *item = (char *)xRingbufferReceive(handle_tmp, &item_size, pdMS_TO_TICKS(1000));

  //Check received item
  if (item != NULL) {
    // Read from buffer
    memcpy(buffer,item,item_size);

    //AddLog(LOG_LEVEL_TEST, PSTR("xRingbufferReceiveUpTo=\"%s\""),buffer);

    // if(flag_clear_buffer_after_read)
    // {
      //Return Item
      vRingbufferReturnItem(handle_tmp, (void *)item); // Free memory
    // }
  } 

  return item_size;

}



#endif
