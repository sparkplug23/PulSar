#include "mPWMLight.h"

#ifdef USE_MODULE_LIGHTS_PWM 

const char* mPWMLight::PM_MODULE_DRIVERS_PWM_CTR = D_MODULE_DRIVERS_PWM_CTR;
const char* mPWMLight::PM_MODULE_DRIVERS_PWM_FRIENDLY_CTR = D_MODULE_DRIVERS_PWM_FRIENDLY_CTR;

int8_t mPWMLight::Tasker(uint8_t function, JsonParserObject obj){}
void mPWMLight::parse_JSONCommand(JsonParserObject obj){}

/**********************************************************************************************************************************
 * ********************************************************************************************************************************
 * **** Hardware interfacing ******************************************************************************************************
 * ********************************************************************************************************************************
 * ********************************************************************************************************************************/

void mPWMLight::SetPixelColorHardware(uint16_t index, RgbcctColor colour_hardware)
{

  if(index > 0) 
  {  
    return; 
  } 
  output_colour = colour_hardware;
  // ALOG_INF(PSTR("SetPixelColorHardware index[%d]=> %d,%d,%d,%d,%d"),index,output_colour.R,output_colour.G,output_colour.B,output_colour.WC,output_colour.WW);

}


RgbcctColor mPWMLight::GetPixelColorHardware(uint16_t index)
{

  // ALOG_INF(PSTR("===============GetPixelColorHardware index[%d]=> %d,%d,%d,%d,%d"),index,output_colour.R,output_colour.G,output_colour.B,output_colour.WC,output_colour.WW);

  return output_colour;

}

void mPWMLight::ShowHardware()
{

  LightSetPWMOutputsRgbcctColor(output_colour);

}




/**********************************************************************************************************************************
 * ********************************************************************************************************************************
 * **** Internal for hardware ******************************************************************************************************
 * ********************************************************************************************************************************
 * ********************************************************************************************************************************/


void mPWMLight::LightSetPWMOutputsRgbcctColor(RgbcctColor colour){

  uint16_t tmp_colour[5];

  tmp_colour[0] = pCONT_iLight->change8to10(colour.R);
  tmp_colour[1] = pCONT_iLight->change8to10(colour.G);
  tmp_colour[2] = pCONT_iLight->change8to10(colour.B);
  tmp_colour[3] = pCONT_iLight->change8to10(colour.WC);
  tmp_colour[4] = pCONT_iLight->change8to10(colour.WW); 

  // ALOG_INF(PSTR("LightSetPWMOutputsRgbcctColor"));
  // AddLog_Array(LOG_LEVEL_TEST,PSTR("tmp_colour"),tmp_colour,(uint16_t)5);

  LightSetPWMOutputsArray10bit(tmp_colour);

}

/**
 * Kept using array for sutypes of other colours
 * */
void mPWMLight::LightSetPWMOutputsArray10bit(const uint16_t *cur_col_10) {
  
  uint16_t cur_col;
  uint16_t pin_num;
  uint16_t pwm_value;
  
  // ALOG_INF(PSTR("subtype %d, pwm_offset %d"), pCONT_iLight->subtype, pCONT_iLight->settings.pwm_offset);
  // ALOG_INF(PSTR("output_colour %d,%d,%d,%d,%d"), output_colour.R,output_colour.G,output_colour.B,output_colour.W1,output_colour.W2);

  // now apply the actual PWM values, adjusted and remapped 10-bits range
  if (pCONT_set->Settings.light_settings.type < LT_PWM6) 
  {
    for (uint8_t i = 0; i < (pCONT_set->Settings.light_settings.type - pCONT_iLight->settings.pwm_offset); i++) 
    {
      if (pCONT_pins->PinUsed(GPIO_PWM1_ID, i)) 
      {
        // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d 10 bits %d"), i, cur_col_10[i]);
        cur_col = cur_col_10[i + pCONT_iLight->settings.pwm_offset]; //leak chance
        
        cur_col = cur_col > 0 ? mapvalue(cur_col, 0, pCONT_set->Settings.pwm_range, pCONT_iLight->pwm_min, pCONT_iLight->pwm_max) : 0;   // shrink to the range of pwm_min..pwm_max
                
        pin_num = pCONT_pins->Pin(GPIO_PWM1_ID, i);
        pwm_value = bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - cur_col : cur_col;
        
        // if(i==0){
        //   ALOG_INF(PSTR("output_colour %d \tcur_col %d\t pwm_value %d"), output_colour.R, cur_col, pwm_value);
        // }

        // ALOG_INF(PSTR("pin_num %d, pwm_value %d"), pin_num, pwm_value);

        analogWrite(pin_num, pwm_value);

      } // Pin used
    } // Subtype
  } // Light type

}



#endif // USE_MODULE_LIGHTS_PWM
