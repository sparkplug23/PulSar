
#include "mLDRBasic.h"

#ifdef USE_MODULE_SENSORS_LDR_BASIC

int8_t mLDRBasic::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();  // should "pre_init" ne chanegd to pin/gpio/hardware pin ??
    break;
    case TASK_INIT:
      Init();
    break;
  }

  switch(function){
    case TASK_LOOP: 
      EveryLoop();
    break;
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT

  }

}



void mLDRBasic::Pre_Init(void)
{

  if(
    tkr_pins->PinUsed(GPIO_LDR_BASIC_DIGITAL1) && 
    tkr_pins->PinUsed(GPIO_LDR_BASIC_ANALOG1)
  ){

    if(tkr_pins->PinUsed(GPIO_LDR_BASIC_DIGITAL1))
    {
      pinMode(tkr_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1), INPUT);
    }
    
    if(tkr_pins->PinUsed(GPIO_LDR_BASIC_ANALOG1))
    {
      pinMode(tkr_pins->GetPin(GPIO_LDR_BASIC_ANALOG1), INPUT);   // This should be changed to configure the analog module, and then just read/return from it
    }

    settings.fEnableSensor = true;
  }

}


void mLDRBasic::Init(void)
{


}



void mLDRBasic::EveryLoop(void)
{


}


void mLDRBasic::EverySecond(void)
{

  ldr[0].analog_reading  = analogRead(tkr_pins->GetPin(GPIO_LDR_BASIC_ANALOG1));
  ldr[0].digital_reading = digitalRead(tkr_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1));

  ALOG_DBM( PSTR("ldr[0].analog_reading =%d %d"), ldr[0].analog_reading, tkr_pins->GetPin(GPIO_LDR_BASIC_ANALOG1));
  ALOG_DBM( PSTR("ldr[0].digital_reading =%d %d"), ldr[0].digital_reading, tkr_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1));

}




/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/



/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mLDRBasic::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_COUNT, settings.fEnableSensor);
    // JBI->Add("RfMask", mySwitch->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mLDRBasic::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    JBI->Object_Start("LDR_Raw");
  
      JBI->Add("analog", ldr[0].analog_reading);
      JBI->Add("digital", ldr[0].digital_reading);
      // JBI->Add(D_RF_BITS, rx_pkt.bit_length);
      // JBI->Add(D_RF_PROTOCOL, rx_pkt.protocol);
      // JBI->Add(D_RF_PULSE, rx_pkt.delay);   
      // JBI->Add(D_MILLIS, rx_pkt.received_time_millis);   
      // JBI->Add(D_TIME, mTime::ConvertU32TimetoCtr(&rx_pkt.received_utc_time, buffer, sizeof(buffer)));
      
    
    JBI->Object_End();
  
  

  return JBI->End();

}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mLDRBasic::MQTTHandler_Init(){

  struct handler<mLDRBasic>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mLDRBasic::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mLDRBasic::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mLDRBasic::ConstructJSON_Sensor;
  
} 

#endif


#endif