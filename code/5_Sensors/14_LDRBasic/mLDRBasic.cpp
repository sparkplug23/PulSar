
#include "mLDRBasic.h"

#ifdef USE_MODULE_SENSORS_LDR_BASIC

int8_t mLDRBasic::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      Pre_Init();  // should "pre_init" ne chanegd to pin/gpio/hardware pin ??
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  switch(function){
    case FUNC_LOOP: 
      EveryLoop();
    break;
    case FUNC_EVERY_SECOND:
      EverySecond();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); 
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT

  }

}



void mLDRBasic::Pre_Init(void)
{

  if(
    pCONT_pins->PinUsed(GPIO_LDR_BASIC_DIGITAL1_ID) && 
    pCONT_pins->PinUsed(GPIO_LDR_BASIC_ANALOG1_ID)
  ){

    if(pCONT_pins->PinUsed(GPIO_LDR_BASIC_DIGITAL1_ID))
    {
      pinMode(pCONT_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1_ID), INPUT);
    }
    
    if(pCONT_pins->PinUsed(GPIO_LDR_BASIC_ANALOG1_ID))
    {
      pinMode(pCONT_pins->GetPin(GPIO_LDR_BASIC_ANALOG1_ID), INPUT);   // This should be changed to configure the analog module, and then just read/return from it
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

  ldr[0].analog_reading  = analogRead(pCONT_pins->GetPin(GPIO_LDR_BASIC_ANALOG1_ID));
  ldr[0].digital_reading = digitalRead(pCONT_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1_ID));

  ALOG_DBM( PSTR("ldr[0].analog_reading =%d %d"), ldr[0].analog_reading, pCONT_pins->GetPin(GPIO_LDR_BASIC_ANALOG1_ID));
  ALOG_DBM( PSTR("ldr[0].digital_reading =%d %d"), ldr[0].digital_reading, pCONT_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1_ID));

}






#endif