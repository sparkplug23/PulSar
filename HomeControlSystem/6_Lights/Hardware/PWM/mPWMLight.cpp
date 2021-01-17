#include "mPWMLight.h"

#ifdef USE_MODULE_LIGHTS_PWM 


/**********************************************************************************************************************************
 * ********************************************************************************************************************************
 * **** Hardware interfacing ******************************************************************************************************
 * ********************************************************************************************************************************
 * ********************************************************************************************************************************/
// SetPixelHardware
/**
 * colour_hardware will already have colour_order mapped
 * */
//flip parameters around so I can leave the index off
void mPWMLight::SetPixelColorHardware(uint16_t index, RgbcctColor colour_hardware){

//addressible will put the colour into the bus

if(index > 0) { return; } //ignore higher colour indexs

output_colour = colour_hardware;


//Serial.printf("red=%d\n\r",output_colour.R);

// pwm will put it into an array (so I can get it back) and set via pwm code


  // pCONT_lAni->stripbus->SetPixelColor(index,colour_hardware);

}


RgbcctColor mPWMLight::GetPixelColorHardware(uint16_t index){

//addressible will put the colour into the bus

return output_colour;

// pwm will put it into an array (so I can get it back) and set via pwm code


  // return pCONT_lAni->stripbus->GetPixelColor(index);

}

void mPWMLight::ShowHardware(){

  // pCONT_lAni->stripbus->Show();
  LightSetPWMOutputsRgbcctColor(output_colour);



}


int8_t mPWMLight::Tasker(uint8_t function){}
void mPWMLight::parse_JSONCommand(void){}


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

  // now apply the actual PWM values, adjusted and remapped 10-bits range
  if (pCONT_set->Settings.light_settings.type < LT_PWM6) {
    for (uint8_t i = 0; i < (pCONT_iLight->subtype - pCONT_iLight->settings.pwm_offset); i++) {
      if (pCONT_pins->PinUsed(GPIO_PWM1_ID, i)) {
        // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d 10 bits %d"), i, cur_col_10[i]);
        cur_col = cur_col_10[i + pCONT_iLight->settings.pwm_offset]; //leak chance
        if(!pCONT_iLight->rgbcct_controller.isChannelCCT(i)) {   // if CT don't use pwm_min and pwm_max
          cur_col = cur_col > 0 ? mapvalue(cur_col, 0, pCONT_set->Settings.pwm_range, pCONT_iLight->pwm_min, pCONT_iLight->pwm_max) : 0;   // shrink to the range of pwm_min..pwm_max
        }
        pin_num = pCONT_pins->Pin(GPIO_PWM1_ID, i);
        pwm_value = bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - cur_col : cur_col;
        // Serial.printf("%d pin_num=%d, %d/%d, %d\n\r", i, pin_num, cur_col, pCONT_set->Settings.pwm_range, pwm_value);
        analogWrite(pin_num, pwm_value);
      } //pin used
    } //subtype
  } //light type

}



#endif // USE_DRIVER
