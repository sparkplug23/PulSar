#include "mSerialUART.h"

/**
 * All Serial buffers will store in ringbuffers, then set a flag that it is ready. Other modules will then "pop" off some of the 
 * data from the buffers and free the memory.
 * */

#ifdef USE_MODULE_DRIVERS_SERIAL_UART

const char* mSerialUART::PM_MODULE_DRIVERS_SERIAL_UART_CTR = D_MODULE_DRIVERS_SERIAL_UART_CTR;
const char* mSerialUART::PM_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR = D_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR;

// static uart_isr_handle_t *handle_console_uart2;
/*********************************************************************************************************************************************************************************************************************************************************************************************************
 ******* INIT **************************************************************************************************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************************************************************************************************* ***************/

void mSerialUART::pre_init(){

//change this into the fan module
//   if(pCONT_pins->PinUsed(GPIO_FAN_SDCARD1_ID)) {  // not set when 255
//     pin = pCONT_pins->GetPin(GPIO_FAN_SDCARD1_ID);
//     pinMode(pin, OUTPUT);
//     settings.fEnableModule = true;
//   }

  settings.uart0.receive_interrupts_enable = false;
  settings.uart0.baud = 115200;

  settings.uart1.receive_interrupts_enable = false;
  settings.uart1.baud = 115200;

  settings.uart2.receive_interrupts_enable = true;
  settings.uart2.baud = 115200;
  settings.uart2.gpio.tx = 17;
  settings.uart2.gpio.rx = 16;

}


void mSerialUART::init(void)
{

  #ifdef ENABLE_HARDWARE_UART_2
  if(settings.uart2.receive_interrupts_enable)
  {
    init_UART2_RingBuffer();
    init_UART2_ISR();
  }
  #endif // ENABLE_HARDWARE_UART_2
  
}


#ifdef ENABLE_HARDWARE_UART_2
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
    settings.uart2.initialised = false; //disable if fault
  }

}

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
      UART_NUM_2,                   // UART Number
      uart2_rxtx_ring_buffer_size,  // UART RX ring buffer size.
      uart2_rxtx_ring_buffer_size,  // UART TX ring buffer size. If set to zero, driver will not use TX buffer, TX function will block task until all data have been sent out.
      uart2_event_queue_size,       // UART event queue size/depth.
      &uart2_event_queue_handle,    // UART event queue handle (out param). On success, a new queue handle is written here to provide access to UART events. If set to NULL, driver will not use an event queue.
      0                             // intr_alloc_flags (esp_intr_alloc.h)
    )
  );

	// // release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(
    uart_isr_free(UART_NUM_2)       // Free UART interrupt handler registered by uart_isr_register. Must be called on the same core as uart_isr_register was called.
  ); 

  uart_isr_register(
    UART_NUM_2,
    uart_intr_handle_u2_static,
    NULL, 
    ESP_INTR_FLAG_IRAM,
    handle_console_uart2
  );
  
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("init_UART2_ISR Started %d buffer size"),uart2_rxtx_ring_buffer_size);

  #ifdef ENABLE_BUILTIN_LED_ACTIVITY_BLINKING
  gpio_pad_select_gpio(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(BLINK_GPIO, LOW); // Turn off by default
  #endif

}


/*
 * Define UART interrupt subroutine to ackowledge interrupt
 */
void IRAM_ATTR uart_intr_handle_u2_static(void *arg)
{ 
  
  uint16_t rx_fifo_len, status;
  uint16_t i = 0; 

  status = UART2.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer
  
  while(rx_fifo_len){
    pCONT_uart->rxbuf2[i++] = UART2.fifo.rw_byte; // read all bytes
    rx_fifo_len--;
  }
  pCONT_uart->urxlen2 = i;

  //^^ Wait, can this just be put into ring, or else create buffer here only for reading

  BaseType_t dummyval;
  UBaseType_t res =  xRingbufferSendFromISR(pCONT_uart->settings.uart2.ringbuffer_handle, pCONT_uart->rxbuf2, pCONT_uart->urxlen2, &dummyval);
  
  if (res != pdTRUE) {
    // AddLog(LOG_LEVEL_ERROR, PSTR("Failed to send item"));
  }

  // after reading bytes from buffer clear UART interrupt status
  uart_clear_intr_status(UART_NUM_2, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
  
  gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));

}

#endif // ENABLE_HARDWARE_UART_2





int8_t mSerialUART::Tasker(uint8_t function, JsonParserObject obj){


// Serial.println();

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  // Only continue in to tasker if module was configured properly
  //if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:{
      
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

      // char buffer[100] = {0};
      // uint16_t bytes_in_line = GetReceiveBuffer2(buffer, sizeof(buffer), '\n');

      // if(buffer){
      //   AddLog(LOG_LEVEL_TEST, PSTR("size=%d, \"%s\""), bytes_in_line, buffer);
      // }else{
      //   AddLog(LOG_LEVEL_TEST, PSTR("size=%d, \"none\""), bytes_in_line);
      // }

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

  // Active rgbcct palette used as scene

  //Serial.println("mSerialUART::ConstructJSON_Settings");

  char buffer[30];
  
  JsonBuilderI->Start();  

  // Got to ConstructJson_Scene out, or rename all the parameters as something else, or rgbcctactivepalette, or show them all? though that would need to run through, can only show
  // active_id, plus the values below
  // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  //   JsonBuilderI->Add_P(PM_JSON_SCENE_NAME, GetSceneName(buffer, sizeof(buffer)));  
  //   #endif //  ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  
    // JsonBuilderI->Add_P(PM_JSON_HUE, rgbcct_controller.getHue360());
    // JsonBuilderI->Add_P(PM_JSON_SAT, rgbcct_controller.getSat255());
    // JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_RGB, rgbcct_controller.getBrightnessRGB255());
    JsonBuilderI->Add_P(PM_JSON_TIME, 1000);
    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}


////////////////////// START OF MQTT /////////////////////////

void mSerialUART::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSerialUART::ConstructJSON_Settings;

  
//   mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SCENE_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mSerialUART::ConstructJSON_Scene;

//   mqtthandler_ptr = &mqtthandler_sensdebug_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mSerialUART::ConstructJSON_Debug;

} //end "MQTTHandler_Init"


void mSerialUART::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  // mqtthandler_animation_teleperiod.flags.SendNow = true;
  // mqtthandler_ambilight_teleperiod.flags.SendNow = true;
//   mqtthandler_scene_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mSerialUART::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // // mqtthandler_animation_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // // mqtthandler_ambilight_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_scene_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  
} //end "MQTTHandler_Set_TelePeriod"


void mSerialUART::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID
    //, MQTT_HANDLER_MODULE_SCENE_TELEPERIOD_ID, MQTT_HANDLER_MODULE_DEBUG_PARAMETERS_TELEPERIOD_ID
  };
  
  struct handler<mSerialUART>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod
    //, &mqtthandler_scene_teleperiod, &mqtthandler_debug_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_DRIVERS_SERIAL_UART_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids, 
    sizeof(mqtthandler_list_ids)/sizeof(mqtthandler_list_ids[0]),
    mqtt_handler_id
  );

}


void mSerialUART::parse_JSONCommand(JsonParserObject obj){

  

//   if(obj.containsKey(D_JSON_FANSPEED)){
//     int speed = obj[D_JSON_FANSPEED];
//     test_val = speed;
//     if(speed>0){
//       analogWrite(pin, test_val);
//     }else
//     if(speed==1023){
//       digitalWrite(pin, HIGH);      
//     }else{
//       digitalWrite(pin, LOW);
//     }
//     // if(speed>3){
//     //   Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_FANSPEED,D_PARSING_NOMATCH);
//     //   speed=0; //default off
//     // }      
//     // SetFanSpeed(speed, false);
//     // AddLog(LOG_LEVEL_INFO,PSTR("GetFanspeed=%d"),GetFanspeed());
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_FANSPEED,speed);
//     // Response_mP(S_JSON_COMMAND_NVALUE,D_JSON_FANSPEED,speed);
//     // isserviced++;
//   }
  
  

}




// int8_t mSerialUART::Tasker_Web(uint8_t function){

//   switch(function){
//     case FUNC_WEB_APPEND_ROOT_BUTTONS:{

//       // create command list
//       // char dlist[100]; memset(dlist,0,sizeof(dlist));
//       // pCONT_sup->AppendDList(dlist, D_JSON_LIGHTPOWER);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);

//       uint8_t button_values[5] = {2, 0, 1, 2, 3}; //toggle, fanspeed0-3
          
//       // BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("Fan Controls"),"");

//       char button_value_ctr[10];
//       char button_key_ctr[50];
//       char button_text_ctr[30];

//       // BufferWriterI->Append_P(PSTR("{t}<tr>"));

//       if(settings.fShowManualSlider){
//         BufferWriterI->Append_P(PSTR("<div> Fan Speed </div>"), test_val);
//         BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
//           WEB_HANDLE_SDCARD_SLIDER,               // c - Unique HTML id
//           PSTR("#000"), PSTR("#eee"),   // Black to White
//           4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
//           0, 1023,  // Range 0/1 to 100%
//           test_val,
//           WEB_HANDLE_SDCARD_SLIDER
//         );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));
//       }

//         // for(uint8_t button_id=0;button_id<5;button_id++){
//         //   BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_INSERTS_HANDLE_IHR, 
//         //                             100/(button_id==0?1:4),
//         //                             button_id==0?"4":"", 
//         //                             "buttonh",
//         //                             pCONT_sup->GetTextIndexed_P(button_key_ctr, sizeof(button_key_ctr), button_id, dlist), 
//         //                             pCONT_sup->p_snprintf(button_value_ctr, sizeof(button_value_ctr), "%d", button_values[button_id]),
//         //                             pCONT_sup->GetTextIndexed_P(button_text_ctr, sizeof(button_text_ctr), button_id, kListFanControls),
//         //                             ""
//         //                         );
//         //   // LightPower button gets its own row
//         //   if(button_id==0){ BufferWriterI->Append_P(PSTR("</tr><tr>")); }
//         // }
//       // BufferWriterI->Append_P(PSTR("</tr>{t2}"));

//     }break; 
//     case FUNC_WEB_COMMAND:
//       WebCommand_Parse();      
//     break;
//   }

// }



// void mSerialUART::WebCommand_Parse(void)
// {
//   AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "mRGBAnimator::WebCommand_Parse"));

//   char tmp[100];

//   uint8_t  arg_value = 0;
//  if(pCONT_web->request_web_command == nullptr){
//   Serial.println("nullptr"); 
//   return; 
// }

//   char arg_ctr[30]; memset(arg_ctr,0,sizeof(arg_ctr));
  
//   sprintf_P(arg_ctr,PSTR(WEB_HANDLE_SDCARD_SLIDER));
//   if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     test_val = arg_value;

//     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//       analogWrite(pin, test_val);
//     // AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
//     // SetRefreshLEDs();
//   }

// }



// uint8_t mSerialUART::ConstructJSON_Settings(uint8_t json_method){
  
//   JsonBuilderI->Start();
//     JsonBuilderI->Add_P("test",0);  
//   JsonBuilderI->End();

// }

// uint8_t mSerialUART::ConstructJSON_Sensor(uint8_t json_method){

//   JsonBuilderI->Start();
//     JsonBuilderI->Add_P(D_JSON_LIGHTPOWER, GetLightState());
//     JsonBuilderI->Add_P(D_JSON_FANSPEED, GetFanspeed());  
//   JsonBuilderI->End();

// }




// /*********************************************************************************************************************************************
// ******** MQTT **************************************************************************************************************************************
// **********************************************************************************************************************************************
// ********************************************************************************************************************************************/

// void mSerialUART::MQTTHandler_Init(){

//   mqtthandler_ptr = &mqtthandler_settings_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 600; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mSerialUART::ConstructJSON_Settings;

//   mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 600; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mSerialUART::ConstructJSON_Sensor;

//   mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mSerialUART::ConstructJSON_Sensor;
  
// } //end "MQTTHandler_Init"


// void mSerialUART::MQTTHandler_Set_fSendNow(){

//   mqtthandler_settings_teleperiod.flags.SendNow = true;
//   mqtthandler_sensor_ifchanged.flags.SendNow = true;
//   mqtthandler_sensor_teleperiod.flags.SendNow = true;

// } //end "MQTTHandler_Init"


// void mSerialUART::MQTTHandler_Set_TelePeriod(){

//   // mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   // mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

// } //end "MQTTHandler_Set_TelePeriod"


// void mSerialUART::MQTTHandler_Sender(uint8_t mqtt_handler_id){

//   uint8_t flag_handle_all = false, handler_found = false
//   if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

//   // change switch to use array of pointers?
//   do{

//     switch(mqtt_handler_id){
//       case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
//       case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
//       case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
//       default: handler_found=false; break; // nothing 
//     } // switch

//     // Pass handlers into command to test and (ifneeded) execute
//     if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_DRIVERS_SDCARD_ID,mqtthandler_ptr); }

//     // stop searching
//     if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

//   }while(flag_handle_all);

// }

////////////////////// END OF MQTT /////////////////////////




// #ifdef USE_DEVFEATURE_UART_TEST
// /* 
// UART Interrupt Example
// */
// #ifdef CORE_DEBUG_LEVEL
// #undef CORE_DEBUG_LEVEL
// #endif

// #define CORE_DEBUG_LEVEL 3
// #define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

// #ifdef USE_DEVFEATURE_RINGBUFFERS
// #include "freertos/ringbuf.h"
// // static char tx_item[] = "test_item";
// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "driver/uart.h"
// #include "esp_log.h"
// #include "driver/gpio.h"
// #include "sdkconfig.h"
// #include "esp_intr_alloc.h"


// #define BUF_HANDLE_LENGTH 10000

// #define BLINK_GPIO GPIO_NUM_2

// // static const char *TAG = "uart_events";

// // Setup UART buffered IO with event queue
// const int uart_buffer_size = (1024 * 2);
// QueueHandle_t uart2_event_queue_handle;


// static QueueHandle_t buffer_queue;
// char buffer_test[100];

//     //Create ring buffer
//     RingbufHandle_t buf_handle;
// #endif
// // #define RINGBUFFER_HANDLE_2_LENGTH 10000
// // struct settings.uart2{
// //   RingbufHandle_t ringbuffer_handle;
// //   uint8_t initialised = false;
// // }settings.uart2;

// /**
//  * This example shows how to use the UART driver to handle UART interrupt.
//  *
//  * - Port: UART0
//  * - Receive (Rx) buffer: on
//  * - Transmit (Tx) buffer: off
//  * - Flow control: off
//  * - Event queue: on
//  * - Pin assignment: TxD (default), RxD (default)
//  */

//   void   init_UART0();
//   void   init_UART1();
//   void   init_UART2();

// #define EX_UART_NUM UART_NUM_0
// #define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

// #define BUF_SIZE (1024)
// #define RD_BUF_SIZE (BUF_SIZE)
// static QueueHandle_t uart0_queue;
// static QueueHandle_t queue_rx_u2;

// // Both definition are same and valid
// //static uart_isr_handle_t *handle_console;
// static intr_handle_t handle_console;
// static intr_handle_t handle_console_u0;


// uint32_t ticktest = millis();
// uint32_t tick_fill = millis();
// uint32_t tick_read = millis();
// uint16_t number = 0;
// uint16_t  freeSize = 0;


// // Receive buffer to collect incoming data
// uint8_t rxbuf[256];
// // Register to collect data length
// uint16_t urxlen;

// #define NOTASK 0

// //https://www.reddit.com/r/esp32/comments/8k6ruk/uart_interrupt_not_working/
// static uart_isr_handle_t *handle_console4;

// uint8_t argISR;

// static const char *TAG4 = "uart_events";

// /**
//  * This example shows how to use the UART driver to handle special UART events.
//  *
//  * It also reads data from UART0 directly, and echoes it to console.
//  *
//  * - Port: UART0
//  * - Receive (Rx) buffer: on
//  * - Transmit (Tx) buffer: off
//  * - Flow control: off
//  * - Event queue: on
//  * - Pin assignment: TxD (default), RxD (default)
//  */
//   struct AMessage
//    {
//   	char ucMessageID;
//   	char ucData[ 20 ];
//    } xMessage;
 
//    QueueHandle_t xQueue;

// // This will run forever! I dont want this, as a thread, use a flag in the main loop instead
// static void uart_event_task(void *pvParameters)
// {
//     uart_event_t event;
//     size_t buffered_size;
//     uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);

     
//     // Task to create a queue and post a value.
//   //  void vATask( void *pvParameters )
//   //  {
// //    struct AMessage *pxMessage;
 
// //   	// Create a queue capable of containing 10 pointers to AMessage structures.
// //   	// These should be passed by pointer as they contain a lot of data.
// //   	xQueue = xQueueCreate( 10, sizeof( struct AMessage * ) );
// //  	  if( xQueue == 0 )
// //   	{
// //   		// Failed to create the queue.
// //   	}

// // //  uart0_queue


// //   	// ...
 
// //   	// Send a pointer to a struct AMessage object.  Don't block if the
// //   	// queue is already full.
// //   	pxMessage = & xMessage;
// //   	xQueueSend( xQueue, ( void * ) &pxMessage, ( TickType_t ) 0 );
 
// //   	// ... Rest of task code.
// //   //  }
 
// //   //  // Task to receive from the queue.
// //   //  void vADifferentTask( void *pvParameters )
// //   //  {
// //    struct AMessage *pxRxedMessage;
 
// //   // 	if( xQueue != 0 )
// //   // 	{
// //   // 		// Receive a message on the created queue.  Block for 10 ticks if a
// //   // 		// message is not immediately available.
// //   		if( xQueueReceive( xQueue, &( pxRxedMessage ), ( TickType_t ) 10 ) )
// //   		{
// //   			// pcRxedMessage now points to the struct AMessage variable posted
// //   			// by vATask.
// //   		}
//   // 	}

//   uint32_t time1 = millis();
  
  
//       Serial.println("start");
  
//     // for(;;) {

//       // yield();
//       // ESP_LOGI(TAG, "uart[%d] event:", UART_NUM_2);

 
//   	// Create a queue capable of containing 10 pointers to AMessage structures.
//   	// These should be passed by pointer as they contain a lot of data.
//   	// uart0_queue = xQueueCreate( 10, sizeof( struct AMessage * ) );
//   	// if( uart0_queue == 0 )
//   	// {
//   	// 	// Failed to create the queue.
//   	// }
  
//   if(abs(millis()-time1)>1000){
//     time1 = millis();
//       Serial.println("for");
//   }
  

// /*

//         //Waiting for UART event.
//         if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
          
//       Serial.println("art[%d] even1");

//             bzero(dtmp, RD_BUF_SIZE);
//             ESP_LOGI(TAG, "uart[%d] event:", UART_NUM_2);
//             switch(event.type) {
//                 //Event of UART receving data
//                 // *We'd better handler data event fast, there would be much more data events than
//                 // other types of events. If we take too much time on data event, the queue might
//                 // be full.*
//                 case UART_DATA:
//                     ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
//                     uart_read_bytes(UART_NUM_2, dtmp, event.size, portMAX_DELAY);
//                     ESP_LOGI(TAG, "[DATA EVT]:");
//                     uart_write_bytes(UART_NUM_2, (const char*) dtmp, event.size);
//                     break;
//                 //Event of HW FIFO overflow detected
//                 // case UART_FIFO_OVF:
//                 //     ESP_LOGI(TAG, "hw fifo overflow");
//                 //     // If fifo overflow happened, you should consider adding flow control for your application.
//                 //     // The ISR has already reset the rx FIFO,
//                 //     // As an example, we directly flush the rx buffer here in order to read more data.
//                 //     uart_flush_input(UART_NUM_2);
//                 //     xQueueReset(uart0_queue);
//                 //     break;
//                 // //Event of UART ring buffer full
//                 // case UART_BUFFER_FULL:
//                 //     ESP_LOGI(TAG, "ring buffer full");
//                 //     // If buffer full happened, you should consider encreasing your buffer size
//                 //     // As an example, we directly flush the rx buffer here in order to read more data.
//                 //     uart_flush_input(UART_NUM_2);
//                 //     xQueueReset(uart0_queue);
//                 //     break;
//                 // //Event of UART RX break detected
//                 // case UART_BREAK:
//                 //     ESP_LOGI(TAG, "uart rx break");
//                 //     break;
//                 // //Event of UART parity check error
//                 // case UART_PARITY_ERR:
//                 //     ESP_LOGI(TAG, "uart parity error");
//                 //     break;
//                 // //Event of UART frame error
//                 // case UART_FRAME_ERR:
//                 //     ESP_LOGI(TAG, "uart frame error");
//                 //     break;
//                 // //UART_PATTERN_DET
//                 // case UART_PATTERN_DET:{
//                 //     uart_get_buffered_data_len(UART_NUM_2, &buffered_size);
//                 //     int pos = uart_pattern_pop_pos(UART_NUM_2);
//                 //     ESP_LOGI(TAG, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
//                 //     if (pos == -1) {
//                 //         // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
//                 //         // record the position. We should set a larger queue size.
//                 //         // As an example, we directly flush the rx buffer here.
//                 //         uart_flush_input(UART_NUM_2);
//                 //     } else {
//                 //         uart_read_bytes(UART_NUM_2, dtmp, pos, 100 / portTICK_PERIOD_MS);
//                 //         uint8_t pat[PATTERN_CHR_NUM + 1];
//                 //         memset(pat, 0, sizeof(pat));
//                 //         uart_read_bytes(UART_NUM_2, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
//                 //         ESP_LOGI(TAG, "read data: %s", dtmp);
//                 //         ESP_LOGI(TAG, "read pat : %s", pat);
//                 //     }
//                 // }
//                 //     break;
//                 //Others
//                 default:
//                     ESP_LOGI(TAG, "uart event type: %d", event.type);
//                     break;
//             }
//         } //queue
//         // yield(); //feed WDT?
        
//     */


//     // } //end for 



//     free(dtmp);
//     dtmp = NULL;
//     vTaskDelete(NULL);
// }




// void blink_task(void *pvParameter)
// {
//     gpio_pad_select_gpio(BLINK_GPIO);
    
//     /* Set the GPIO as a push/pull output */
//     gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    
//     while(1) {
//         /* Blink off (output low) */
//         gpio_set_level(BLINK_GPIO, 0);
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//         /* Blink on (output high) */
//         gpio_set_level(BLINK_GPIO, 1);
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//   //        uart_write_bytes(
//   // //  EX_UART_NUM, 
//   //  UART_NUM_0,
//   //  (const char*) "blink_task", 7);

//     }
// }
// // /*
// //  * Define UART interrupt subroutine to ackowledge interrupt
// //  */
// // static void IRAM_ATTR uart_intr_handle(void *arg)
// // {
// //   uint16_t rx_fifo_len, status;
// //   uint16_t i;
  
// //     gpio_pad_select_gpio(BLINK_GPIO);
    
// //     /* Set the GPIO as a push/pull output */
// //     gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


// //   status = UART2.int_st.val; // read UART interrupt Status
// //   rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer
  
// //   while(rx_fifo_len){
// //    rxbuf[i++] = UART2.fifo.rw_byte; // read all bytes
// //    rx_fifo_len--;
// //         gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
// //  }
    
// //         // /* Blink off (output low) */
// //         // gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
// //         /* Blink on (output high) */
// //         // gpio_set_level(BLINK_GPIO, 1);
  
// // //  // after reading bytes from buffer clear UART interrupt status
// //  uart_clear_intr_status(EX_UART_NUM, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// // // // a test code or debug code to indicate UART receives successfully,
// // // // you can redirect received byte as echo also
// //  uart_write_bytes(
// //   //  EX_UART_NUM, 
// //    UART_NUM_0,
// //    (const char*) "RX Done", 7);

// // }

// /*
//  * Define UART interrupt subroutine to ackowledge interrupt
//  */
// static void IRAM_ATTR uart_intr_handle_u0(void *arg)
// {
//   uint16_t rx_fifo_len, status;
//   uint16_t i;
  
//   gpio_pad_select_gpio(BLINK_GPIO);
    
//   /* Set the GPIO as a push/pull output */
//   gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


//   status = UART0.int_st.val; // read UART interrupt Status
//   rx_fifo_len = UART0.status.rxfifo_cnt; // read number of bytes in UART buffer
//   char single_byte = '-';
  
//   while(rx_fifo_len){
//     single_byte = UART0.fifo.rw_byte;
//     rxbuf[i++] = single_byte; // read all bytes
//     rx_fifo_len--;
//     gpio_set_level(BLINK_GPIO, rx_fifo_len%2);
//   }
    
//         // /* Blink off (output low) */
//         // gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//         /* Blink on (output high) */
//         // gpio_set_level(BLINK_GPIO, 1);
  
// //  // after reading bytes from buffer clear UART interrupt status
//  uart_clear_intr_status(UART_NUM_0, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// // // a test code or debug code to indicate UART receives successfully,
// // // you can redirect received byte as echo also
// //  uart_write_bytes(
// //   //  EX_UART_NUM, 
// //    UART_NUM_2,
// //    (const char*) "RX Done", 7);

// // uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);


//    uart_write_bytes(
//    UART_NUM_0,
//    (const char*) "Setup 0isr, port uart0 workin\n\r", 32);
//    uart_write_bytes(
//    UART_NUM_2,
//    (const char*) "Setup 0isr, port uart2 workin\n\r", 32);
// //  uart_write_bytes(
// //    UART_NUM_0,
// //    (const char*) single_byte, 1);

// }

// /*
//  * Define UART interrupt subroutine to ackowledge interrupt
//  */
// static void IRAM_ATTR uart_intr_handle_u1(void *arg)
// {
//   uint16_t rx_fifo_len, status;
//   uint16_t i;
  
//     gpio_pad_select_gpio(BLINK_GPIO);
    
//     /* Set the GPIO as a push/pull output */
//     gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


//   status = UART1.int_st.val; // read UART interrupt Status
//   rx_fifo_len = UART1.status.rxfifo_cnt; // read number of bytes in UART buffer
//   char single_byte = '-';
  
//   while(rx_fifo_len){
//     single_byte = UART1.fifo.rw_byte;
//     rxbuf[i++] = single_byte; // read all bytes
//     rx_fifo_len--;
//     gpio_set_level(BLINK_GPIO, rx_fifo_len%2);
//   }
    
//         // /* Blink off (output low) */
//         // gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//         /* Blink on (output high) */
//         // gpio_set_level(BLINK_GPIO, 1);
  
// //  // after reading bytes from buffer clear UART interrupt status
//  uart_clear_intr_status(UART_NUM_1, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// // // a test code or debug code to indicate UART receives successfully,
// // // you can redirect received byte as echo also
// //  uart_write_bytes(
// //   //  EX_UART_NUM, 
// //    UART_NUM_2,
// //    (const char*) "RX Done", 7);

// // uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);


//    uart_write_bytes(
//    UART_NUM_0,
//    (const char*) "Setup 1isr, port uart0 workin\n\r", 32);
//    uart_write_bytes(
//    UART_NUM_2,
//    (const char*) "Setup 1isr, port uart2 workin\n\r", 32);
// //  uart_write_bytes(
// //    UART_NUM_0,
// //    (const char*) single_byte, 1);

// }

// /*
//  * Define UART interrupt subroutine to ackowledge interrupt
//  */
// static void IRAM_ATTR uart_intr_handle_u2(void *arg)
// {
//   uint16_t rx_fifo_len, status;
//   uint16_t i = 0;
  
//   // uart_write_bytes(
//   //  UART_NUM_1,
//   //  (const char*) "UART2 - uart_intr_handle_u2\n\r", 30);



//     gpio_pad_select_gpio(BLINK_GPIO);
    
//     /* Set the GPIO as a push/pull output */
//     gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


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
//   // for(int ii=0;ii<i;ii++)
//     // if(i<30){
//       // uart_write_bytes(   UART_NUM_1,   (const char*) rxbuf, i-1);
//     // } 

    
//     // char message[100] = {0};
//     // for(int i=0;i<9;i++){
//     //   sprintf(message+strlen(message),"U2_%d",number);
//     // }
//     // number++;

// // BaseType_t dummyval;

// //     UBaseType_t res =  xRingbufferSendFromISR(settings.uart2.ringbuffer_handle, rxbuf, urxlen, &dummyval);
    
// //     if (res != pdTRUE) {
// //       Serial.printf("Failed to send item\n\r");
// //     }else{
// //       // Serial.printf("messaged added\n\r");
// //     }



//         // /* Blink off (output low) */
//         // gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//         /* Blink on (output high) */
//         // gpio_set_level(BLINK_GPIO, 1);
  
// //  // after reading bytes from buffer clear UART interrupt status
//  uart_clear_intr_status(UART_NUM_2, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// // // a test code or debug code to indicate UART receives successfully,
// // // you can redirect received byte as echo also
// //  uart_write_bytes(
// //   //  EX_UART_NUM, 
// //    UART_NUM_2,
// //    (const char*) "RX Done", 7);

// // uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);


//   //  uart_write_bytes(
//   //  UART_NUM_0,
//   //  (const char*) "Setup 2isr, port uart0 workin\n\r", 32);
//   //  uart_write_bytes(
//   //  UART_NUM_2,
//   //  (const char*) "Setup 2isr, port uart2 workin\n\r", 32);

// //  uart_write_bytes(
// //    UART_NUM_0,
// //    (const char*) single_byte, 1);

//   // uart_write_bytes(
//   //  UART_NUM_1,
//   //  (const char*) "UART2 - isr END\n\r", 18);
// }


// /*
//  * Define UART interrupt subroutine to ackowledge interrupt
//  */
// static void IRAM_ATTR uart_intr_handle_u2_q(void *arg)
// {
//   uint16_t rx_fifo_len, status;
//   uint16_t i = 0;
  
//   uart_write_bytes(
//    UART_NUM_1,
//    (const char*) "UART2 - uart_intr_handle_u2_q\n\r", 32);

// // Read data from UART.
// // const int uart_num = UART_NUM_2;
// // uint8_t data[128];
// // int length = 0;
// // ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));
// // length = uart_read_bytes(uart_num, data, length, 100);

// //If the data in the Rx FIFO buffer is no longer needed, you can clear the buffer by calling uart_flush().


//     // gpio_pad_select_gpio(BLINK_GPIO);
    
//     // /* Set the GPIO as a push/pull output */
//     // gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


// //   status = UART2.int_st.val; // read UART interrupt Status
// //   rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer
// //   char single_byte = '-';
  
// //   while(rx_fifo_len){
// //     single_byte = UART2.fifo.rw_byte;
// //     rxbuf[i++] = single_byte; // read all bytes
// //     rx_fifo_len--;
// //     gpio_set_level(BLINK_GPIO, rx_fifo_len%2);
// //     // uart_tx_chars(UART_NUM_1,  (const char*)single_byte, 1);
// //     // uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);
// //   }
// // urxlen = i;
//   // for(int ii=0;ii<i;ii++)
//     // if(i<30){
//       // uart_write_bytes(   UART_NUM_1,   (const char*) rxbuf, i-1);
//     // } 
//         // /* Blink off (output low) */
//         // gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//         /* Blink on (output high) */
//         // gpio_set_level(BLINK_GPIO, 1);
  
// //  // after reading bytes from buffer clear UART interrupt status
//  uart_clear_intr_status(UART_NUM_2, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// // // a test code or debug code to indicate UART receives successfully,
// // // you can redirect received byte as echo also
// //  uart_write_bytes(
// //   //  EX_UART_NUM, 
// //    UART_NUM_2,
// //    (const char*) "RX Done", 7);

// // uart_tx_chars(UART_NUM_2,  (const char*)single_byte, 1);


//   //  uart_write_bytes(
//   //  UART_NUM_0,
//   //  (const char*) "Setup 2isr, port uart0 workin\n\r", 32);
//   //  uart_write_bytes(
//   //  UART_NUM_2,
//   //  (const char*) "Setup 2isr, port uart2 workin\n\r", 32);

// //  uart_write_bytes(
// //    UART_NUM_0,
// //    (const char*) single_byte, 1);

//   uart_write_bytes(
//    UART_NUM_1,
//    (const char*) "UART2 - isr ENDq\n\r", 19);
// }



// void UART_Setup();
// void UART_Loop();
// #endif // USE_DEVFEATURE_UART_TEST
#endif
