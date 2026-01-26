
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
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
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






#endif