#include "mSerialUART.h"

#ifdef USE_MODULE_DRIVERS_SERIAL_UART



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

    // JBI->Level_Start("UART1");
    //   JBI->Add("receive_interrupts_enable", settings.uart1.receive_interrupts_enable);
      
    //   JBI->Array_Start("Buffer1");
    //     for(int i=0;i<100;i++)
    //       JBI->Add(rxbuf1[i]);
    //   JBI->Array_End();
      

    // JBI->Level_End();

    JBI->Level_Start("UART2");
      JBI->Add("receive_interrupts_enable", settings.uart2.receive_interrupts_enable);
      
      JBI->Array_Start("Buffer1");
        for(int i=0;i<100;i++)
          JBI->Add(rxbuf2[i]);
      JBI->Array_End();
      

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
