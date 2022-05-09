

#include "mPWM.h"

#ifdef USE_MODULE_DRIVERS_PWM

const char* mPWM::PM_MODULE_DRIVERS_PWM_CTR = D_MODULE_DRIVERS_PWM_CTR;
const char* mPWM::PM_MODULE_DRIVERS_PWM_FRIENDLY_CTR = D_MODULE_DRIVERS_PWM_FRIENDLY_CTR;

int8_t mPWM::Tasker(uint8_t function, JsonParserObject obj)
{

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

  switch(function){
    case FUNC_LOOP: 
      EveryLoop();
    break;
    case FUNC_EVERY_SECOND:
      EverySecond();
    break;
    case FUNC_EVERY_50_MSECOND:    {


    /**
     * @brief Copy neopixelanimator to create my own class, that uses template, and can either set delta to transition between numbers, or a time period
     * 
     */

      // if(pwm[0].value==0){
      //   dir = 1;
      // }else
      // if(pwm[0].value>=1023){
      //   dir = 0;
      // }

      // if(dir==1){
      //   pwm[0].value+=1;
      // }else{
      //   pwm[0].value-=1;
      // }

      
  // for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  // {
  //     analogWrite(pwm[i].pin,  pwm[i].value);

  // }

//     uint16_t pwmv = variable_blend->GetTarget();



// analogWrite(16,  pwmv);


      // analogWrite(pwm[0].pin, pwm[0].value);
    }
    break;
    case FUNC_EVERY_FIVE_SECOND:
// ALOG_INF("Starting test blender");

//     var_blend->SetTarget(random(1,10));

//     var_blend->StartAnimation(0, 1000);//, nullptr);
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
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
      WebAppend_Root_Draw_Table();
    break; 
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break; 
    #endif //USE_MODULE_NETWORK_WEBSERVER

  }

}



void mPWM::Pre_Init(void)
{

  // if(
  //   pCONT_pins->PinUsed(GPIO_LDR_BASIC_DIGITAL1_ID) && 
  //   pCONT_pins->PinUsed(GPIO_LDR_BASIC_ANALOG1_ID)
  // ){

  //   if(pCONT_pins->PinUsed(GPIO_LDR_BASIC_DIGITAL1_ID))
  //   {
  //     pinMode(pCONT_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1_ID), INPUT);
  //   }
    
  for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  {
    if(pCONT_pins->PinUsed(GPIO_PWM1_ID,i))
    {
      pinMode(pCONT_pins->GetPin(GPIO_PWM1_ID,i), OUTPUT);   // This should be changed to configure the analog module, and then just read/return from it
      pwm[i].pin = pCONT_pins->GetPin(GPIO_PWM1_ID,i);

    }
  }



    settings.fEnableSensor = true;
  // }

}


void mPWM::Init(void)
{

  /**
   * @brief Note I could also add flag to change the scale here to much longer than 1 minute
   * 
   */
  for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  {
    if(pCONT_pins->PinUsed(GPIO_PWM1_ID,i))
    {
      pwm[i].blended_value = new LinearBlendVariable<uint16_t>(1, BLEND_DATA_MILLISECONDS);
    }
  }

  // var_blend_f  = new LinearBlendVariable<float>(1, BLEND_DATA_MILLISECONDS);
  // var_blend_u8 = new LinearBlendVariable<uint8_t>(1, BLEND_DATA_MILLISECONDS);

}



void mPWM::EveryLoop(void)
{

  // var_blend_f ->UpdateBlend();
  // var_blend_u8->UpdateBlend();

  
  for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  {
    if(pwm[i].blended_value != nullptr)
    {
      pwm[i].blended_value->UpdateBlend();
    }
  }

  /**
   * @brief Update PWM outputs
   * 
   */
  for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  {
    if(pCONT_pins->PinUsed(GPIO_PWM1_ID,i))
    {
      analogWrite(pwm[i].pin, pwm[i].blended_value->GetValue());
    }
  }


}


void mPWM::EverySecond(void)
{

  // ldr[0].analog_reading  = analogRead(pCONT_pins->GetPin(GPIO_LDR_BASIC_ANALOG1_ID));
  // ldr[0].digital_reading = digitalRead(pCONT_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1_ID));

  // AddLog(LOG_LEVEL_TEST, PSTR("ldr[0].analog_reading =%d %d"), ldr[0].analog_reading, pCONT_pins->GetPin(GPIO_LDR_BASIC_ANALOG1_ID));
  // AddLog(LOG_LEVEL_TEST, PSTR("ldr[0].digital_reading =%d %d"), ldr[0].digital_reading, pCONT_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1_ID));

  // for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  // {
  //   AddLog(LOG_LEVEL_INFO, PSTR("pwm[%d].blended_value->UpdateBlend() = %d"), i, pwm[i].blended_value->GetValue());
  // }

}






#endif