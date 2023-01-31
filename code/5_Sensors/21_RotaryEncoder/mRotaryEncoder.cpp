#include "mRotaryEncoder.h" 


#ifdef USE_MODULE_SENSORS_ROTARY_ENCODER 

const char* mRotaryEncoder::PM_MODULE_SENSORS_ROTARY_ENCODER_CTR = D_MODULE_SENSORS_ROTARY_ENCODER_CTR;
const char* mRotaryEncoder::PM_MODULE_SENSORS_ROTARY_ENCODER_FRIENDLY_CTR = D_MODULE_SENSORS_ROTARY_ENCODER_FRIENDLY_CTR;

int8_t mRotaryEncoder::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_LOOP:
      EveryLoop_ReadSensor();
    break;
    case FUNC_EVERY_SECOND:{
      ALOG_INF( PSTR("Encoder: %d"), data.value );
    }break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
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
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mRotaryEncoder::Pre_Init(void)
{

  int8_t pinA = -1, pinB = -1;

  if (pCONT_pins->PinUsed(GPIO__ROTARY_ENCODER_A__ID) && pCONT_pins->PinUsed(GPIO__ROTARY_ENCODER_B__ID))
  {

    pinA = pCONT_pins->GetPin(GPIO__ROTARY_ENCODER_A__ID);
    pinB = pCONT_pins->GetPin(GPIO__ROTARY_ENCODER_B__ID);

    ALOG_INF( PSTR("mRotaryEncoder: A[%d] B[%d]"), pinA, pinB );
  
    encoder = new ESP32Encoder();

    // Enable the weak pull down resistors
    //ESP32Encoder::useInternalWeakPullResistors=DOWN;

    // Enable the weak pull up resistors
    ESP32Encoder::useInternalWeakPullResistors=UP;

    // use pin 19 and 18 for the first encoder
    encoder->attachHalfQuad(pinA, pinB);
      
    // set starting count value after attaching
    encoder->setCount(0);

    // clear the encoder's raw count and set the tracked count to zero
    // encoder2.clearCount();
    
    settings.fEnableSensor = true;
  }
  

}


void mRotaryEncoder::Init(void)
{
  settings.fEnableSensor = true;



}

void mRotaryEncoder::EveryLoop_ReadSensor()
{

  data.value = encoder->getCount();

}





uint8_t mRotaryEncoder::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mRotaryEncoder::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[80];

  JsonBuilderI->Start();
  
    JsonBuilderI->Add("value", data.value);

  return JsonBuilderI->End();
    
}



#endif
